#include "mapnik_VectorTile.h"
//
#include <mapnik/geom_util.hpp>
#include <mapnik/hit_test_filter.hpp>

#include "globals.hpp"

namespace {

struct query_lonlat {
    double lon;
    double lat;
};

struct query_result {
    std::string layer;
    double distance;
    double x_hit;
    double y_hit;
    mapnik::feature_ptr feature;
    explicit query_result() : layer(), distance(0), x_hit(0), y_hit(0) {}
};

struct query_hit {
    double distance;
    unsigned feature_id;
};

struct queryMany_result {
    std::map<unsigned, query_result> features;
    std::map<unsigned, std::vector<query_hit>> hits;
};

struct p2p_result {
    double distance = -1;
    double x_hit = 0;
    double y_hit = 0;
};

struct p2p_distance {
    p2p_distance(double x, double y) : x_(x), y_(y) {}

    p2p_result operator()(mapnik::geometry::geometry_empty const&) const {
        p2p_result p2p;
        return p2p;
    }

    p2p_result operator()(mapnik::geometry::point<double> const& geom) const {
        p2p_result p2p;
        p2p.x_hit = geom.x;
        p2p.y_hit = geom.y;
        p2p.distance = mapnik::distance(geom.x, geom.y, x_, y_);
        return p2p;
    }
    p2p_result operator()(mapnik::geometry::multi_point<double> const& geom) const {
        p2p_result p2p;
        for (auto const& pt : geom) {
            p2p_result p2p_sub = operator()(pt);
            if (p2p_sub.distance >= 0 && (p2p.distance < 0 || p2p_sub.distance < p2p.distance)) {
                p2p.x_hit = p2p_sub.x_hit;
                p2p.y_hit = p2p_sub.y_hit;
                p2p.distance = p2p_sub.distance;
            }
        }
        return p2p;
    }
    p2p_result operator()(mapnik::geometry::line_string<double> const& geom) const {
        p2p_result p2p;
        auto num_points = geom.size();
        if (num_points > 1) {
            for (std::size_t i = 1; i < num_points; ++i) {
                auto const& pt0 = geom[i - 1];
                auto const& pt1 = geom[i];
                double dist = mapnik::point_to_segment_distance(x_, y_, pt0.x, pt0.y, pt1.x, pt1.y);
                if (dist >= 0 && (p2p.distance < 0 || dist < p2p.distance)) {
                    p2p.x_hit = pt0.x;
                    p2p.y_hit = pt0.y;
                    p2p.distance = dist;
                }
            }
        }
        return p2p;
    }
    p2p_result operator()(mapnik::geometry::multi_line_string<double> const& geom) const {
        p2p_result p2p;
        for (auto const& line : geom) {
            p2p_result p2p_sub = operator()(line);
            if (p2p_sub.distance >= 0 && (p2p.distance < 0 || p2p_sub.distance < p2p.distance)) {
                p2p.x_hit = p2p_sub.x_hit;
                p2p.y_hit = p2p_sub.y_hit;
                p2p.distance = p2p_sub.distance;
            }
        }
        return p2p;
    }
    p2p_result operator()(mapnik::geometry::polygon<double> const& poly) const {
        p2p_result p2p;
        std::size_t num_rings = poly.size();
        bool inside = false;
        for (std::size_t ring_index = 0; ring_index < num_rings; ++ring_index) {
            auto const& ring = poly[ring_index];
            auto num_points = ring.size();
            if (num_points < 4) {
                if (ring_index == 0)  // exterior
                    return p2p;
                else                  // interior
                    continue;
            }
            for (std::size_t index = 1; index < num_points; ++index) {
                auto const& pt0 = ring[index - 1];
                auto const& pt1 = ring[index];
                // todo - account for tolerance
                if (mapnik::detail::pip(pt0.x, pt0.y, pt1.x, pt1.y, x_, y_)) { inside = !inside; }
            }
            if (ring_index == 0 && !inside) return p2p;
        }
        if (inside) p2p.distance = 0;
        return p2p;
    }

    p2p_result operator()(mapnik::geometry::multi_polygon<double> const& geom) const {
        p2p_result p2p;
        for (auto const& poly : geom) {
            p2p_result p2p_sub = operator()(poly);
            if (p2p_sub.distance >= 0 && (p2p.distance < 0 || p2p_sub.distance < p2p.distance)) {
                p2p.x_hit = p2p_sub.x_hit;
                p2p.y_hit = p2p_sub.y_hit;
                p2p.distance = p2p_sub.distance;
            }
        }
        return p2p;
    }
    p2p_result operator()(mapnik::geometry::geometry_collection<double> const& collection) const {
        // There is no current way that a geometry collection could be returned from a vector tile.
        // LCOV_EXCL_START
        p2p_result p2p;
        for (auto const& geom : collection) {
            p2p_result p2p_sub = mapnik::util::apply_visitor((*this), geom);
            if (p2p_sub.distance >= 0 && (p2p.distance < 0 || p2p_sub.distance < p2p.distance)) {
                p2p.x_hit = p2p_sub.x_hit;
                p2p.y_hit = p2p_sub.y_hit;
                p2p.distance = p2p_sub.distance;
            }
        }
        return p2p;
        // LCOV_EXCL_STOP
    }

    double x_;
    double y_;
};

p2p_result path_to_point_distance(mapnik::geometry::geometry<double> const& geom, double x, double y) {
    return mapnik::util::apply_visitor(p2p_distance(x, y), geom);
}

std::vector<query_result> _query(mapnik::vector_tile_impl::merc_tile_ptr const& tile, double lon, double lat,
                                 double tolerance, std::string const& layer_name) {
    std::vector<query_result> arr;
    if (tile->is_empty()) { return arr; }

    mapnik::projection wgs84("epsg:4326", true);
    mapnik::projection merc("epsg:3857", true);
    mapnik::proj_transform tr(wgs84, merc);
    double x = lon;
    double y = lat;
    double z = 0;
    if (!tr.forward(x, y, z)) {
        // THIS CAN NEVER BE REACHED CURRENTLY
        // internally lonlat2merc in mapnik can never return false.
        // LCOV_EXCL_START
        throw std::runtime_error("could not reproject lon/lat to mercator");
        // LCOV_EXCL_STOP
    }

    mapnik::coord2d pt(x, y);
    if (!layer_name.empty()) {
        protozero::pbf_reader layer_msg;
        if (tile->layer_reader(layer_name, layer_msg)) {
            auto ds = std::make_shared<mapnik::vector_tile_impl::tile_datasource_pbf>(layer_msg, tile->x(), tile->y(),
                                                                                      tile->z());
            mapnik::featureset_ptr fs = ds->features_at_point(pt, tolerance);
            if (fs && mapnik::is_valid(fs)) {
                mapnik::feature_ptr feature;
                while ((feature = fs->next())) {
                    auto const& geom = feature->get_geometry();
                    auto p2p = path_to_point_distance(geom, x, y);
                    if (!tr.backward(p2p.x_hit, p2p.y_hit, z)) {
                        // LCOV_EXCL_START
                        throw std::runtime_error("could not reproject lon/lat to mercator");
                        // LCOV_EXCL_STOP
                    }
                    if (p2p.distance >= 0 && p2p.distance <= tolerance) {
                        query_result res;
                        res.x_hit = p2p.x_hit;
                        res.y_hit = p2p.y_hit;
                        res.distance = p2p.distance;
                        res.layer = layer_name;
                        res.feature = feature;
                        arr.push_back(std::move(res));
                    }
                }
            }
        }
    } else {
        protozero::pbf_reader item(tile->get_reader());
        while (item.next(mapnik::vector_tile_impl::Tile_Encoding::LAYERS)) {
            protozero::pbf_reader layer_msg = item.get_message();
            auto ds = std::make_shared<mapnik::vector_tile_impl::tile_datasource_pbf>(layer_msg, tile->x(), tile->y(),
                                                                                      tile->z());
            mapnik::featureset_ptr fs = ds->features_at_point(pt, tolerance);
            if (fs && mapnik::is_valid(fs)) {
                mapnik::feature_ptr feature;
                while ((feature = fs->next())) {
                    auto const& geom = feature->get_geometry();
                    auto p2p = path_to_point_distance(geom, x, y);
                    if (!tr.backward(p2p.x_hit, p2p.y_hit, z)) {
                        // LCOV_EXCL_START
                        throw std::runtime_error("could not reproject lon/lat to mercator");
                        // LCOV_EXCL_STOP
                    }
                    if (p2p.distance >= 0 && p2p.distance <= tolerance) {
                        query_result res;
                        res.x_hit = p2p.x_hit;
                        res.y_hit = p2p.y_hit;
                        res.distance = p2p.distance;
                        res.layer = ds->get_name();
                        res.feature = feature;
                        arr.push_back(std::move(res));
                    }
                }
            }
        }
    }
    std::sort(arr.begin(), arr.end(),
              [](query_result const& a, query_result const& b) { return a.distance < b.distance; });
    return arr;
}

jobject _queryResultToJNI(JNIEnv* env, std::vector<query_result>& result) {
    jobject out = env->AllocObject(CLASS_VECTOR_TILE_QUERY_RESULT);
    JNIObject arr(env, env->NewObjectArray(result.size(), CLASS_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE, NULL));
    jsize i = 0;
    for (auto& item : result) {
        JNIObject obj(env, env->AllocObject(CLASS_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE));
        JNIObject layer(env, env->NewStringUTF(item.layer.c_str()));
        auto feature = new mapnik::feature_ptr();
        feature->swap(item.feature);
        JNIObject featurej(env, createFeatureObj(env, feature));
        env->SetObjectField(obj.get(), FIELD_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE_LAYER, layer.get());
        env->SetDoubleField(obj.get(), FIELD_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE_DISTANCE, item.distance);
        env->SetDoubleField(obj.get(), FIELD_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE_XHIT, item.x_hit);
        env->SetDoubleField(obj.get(), FIELD_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE_YHIT, item.y_hit);
        env->SetObjectField(obj.get(), FIELD_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE_FEATURE, featurej.get());
        env->SetObjectArrayElement((jobjectArray)arr.get(), i++, obj.get());
    }
    env->SetObjectField(out, FIELD_VECTOR_TILE_QUERY_RESULT_FEATURES, arr.get());
    return out;
}

void _queryMany(queryMany_result& result, mapnik::vector_tile_impl::merc_tile_ptr const& tile,
                std::vector<query_lonlat> const& query, double tolerance, std::string const& layer_name,
                std::vector<std::string> const& fields) {
    protozero::pbf_reader layer_msg;
    if (!tile->layer_reader(layer_name, layer_msg)) { throw std::runtime_error("Could not find layer in vector tile"); }

    std::map<unsigned, query_result> features;
    std::map<unsigned, std::vector<query_hit>> hits;

    // Reproject query => mercator points
    mapnik::box2d<double> bbox;
    mapnik::projection wgs84("epsg:4326", true);
    mapnik::projection merc("epsg:3857", true);
    mapnik::proj_transform tr(wgs84, merc);
    std::vector<mapnik::coord2d> points;
    points.reserve(query.size());
    for (std::size_t p = 0; p < query.size(); ++p) {
        double x = query[p].lon;
        double y = query[p].lat;
        double z = 0;
        if (!tr.forward(x, y, z)) {
            // LCOV_EXCL_START
            throw std::runtime_error("could not reproject lon/lat to mercator");
            // LCOV_EXCL_STOP
        }
        mapnik::coord2d pt(x, y);
        bbox.expand_to_include(pt);
        points.emplace_back(std::move(pt));
    }
    bbox.pad(tolerance);

    std::shared_ptr<mapnik::vector_tile_impl::tile_datasource_pbf> ds =
        std::make_shared<mapnik::vector_tile_impl::tile_datasource_pbf>(layer_msg, tile->x(), tile->y(), tile->z());
    mapnik::query q(bbox);
    if (fields.empty()) {
        // request all data attributes
        auto fields2 = ds->get_descriptor().get_descriptors();
        for (auto const& field : fields2) { q.add_property_name(field.get_name()); }
    } else {
        for (std::string const& name : fields) { q.add_property_name(name); }
    }
    mapnik::featureset_ptr fs = ds->features(q);

    if (fs && mapnik::is_valid(fs)) {
        mapnik::feature_ptr feature;
        unsigned idx = 0;
        while ((feature = fs->next())) {
            unsigned has_hit = 0;
            for (std::size_t p = 0; p < points.size(); ++p) {
                mapnik::coord2d const& pt = points[p];
                auto const& geom = feature->get_geometry();
                auto p2p = path_to_point_distance(geom, pt.x, pt.y);
                if (p2p.distance >= 0 && p2p.distance <= tolerance) {
                    has_hit = 1;
                    query_result res;
                    res.feature = feature;
                    res.distance = 0;
                    res.layer = ds->get_name();

                    query_hit hit;
                    hit.distance = p2p.distance;
                    hit.feature_id = idx;

                    features.insert(std::make_pair(idx, res));

                    std::map<unsigned, std::vector<query_hit>>::iterator hits_it;
                    hits_it = hits.find(p);
                    if (hits_it == hits.end()) {
                        std::vector<query_hit> pointHits;
                        pointHits.reserve(1);
                        pointHits.push_back(std::move(hit));
                        hits.insert(std::make_pair(p, pointHits));
                    } else {
                        hits_it->second.push_back(std::move(hit));
                    }
                }
            }
            if (has_hit > 0) { idx++; }
        }
    }

    // Sort each group of hits by distance.
    for (auto& hit : hits) {
        std::sort(hit.second.begin(), hit.second.end(),
                  [](auto const& a, auto const& b) { return a.distance < b.distance; });
    }

    result.hits = std::move(hits);
    result.features = std::move(features);
}

jobject _queryManyResultToJNI(JNIEnv* env, queryMany_result& result) {
    jobject results = env->AllocObject(CLASS_VECTOR_TILE_QUERY_MANY_RESULT);
    JNIObject features(
        env, env->NewObjectArray(result.features.size(), CLASS_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_FEATURE, NULL));
    JNIObject hits(env, env->NewObjectArray(result.hits.size(), CLASS_OBJECT_ARRAY, NULL));
    env->SetObjectField(results, FIELD_VECTOR_TILE_QUERY_MANY_RESULT_FEATURES, features.get());
    env->SetObjectField(results, FIELD_VECTOR_TILE_QUERY_MANY_RESULT_HITS, hits.get());

    for (auto& item : result.features) {
        JNIObject obj(env, env->AllocObject(CLASS_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_FEATURE));
        JNIObject layer(env, env->NewStringUTF(item.second.layer.c_str()));
        auto feature = new mapnik::feature_ptr();
        feature->swap(item.second.feature);
        JNIObject featurej(env, createFeatureObj(env, feature));
        env->SetObjectField(obj.get(), FIELD_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_FEATURE_LAYER, layer.get());
        env->SetObjectField(obj.get(), FIELD_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_FEATURE_FEATURE, featurej.get());
        env->SetObjectArrayElement((jobjectArray)features.get(), item.first, obj.get());
    }

    for (auto const& hit : result.hits) {
        JNIObject point_hits(
            env, env->NewObjectArray(hit.second.size(), CLASS_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_HIT, NULL));
        jsize i = 0;
        for (auto const& h : hit.second) {
            JNIObject hit_obj(env, env->AllocObject(CLASS_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_HIT));
            env->SetDoubleField(hit_obj.get(), FIELD_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_HIT_DISTANCE, h.distance);
            env->SetIntField(hit_obj.get(), FIELD_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_HIT_FEATURE_ID, h.feature_id);
            env->SetObjectArrayElement((jobjectArray)point_hits.get(), i++, hit_obj.get());
        }
        env->SetObjectArrayElement((jobjectArray)hits.get(), hit.first, point_hits.get());
    }
    return results;
}

}  // namespace

/*
 * Class:     mapnik_VectorTile
 * Method:    queryImpl
 * Signature: (DDDLjava/lang/String;)Lmapnik/VectorTile/QueryResult;
 */
JNIEXPORT jobject JNICALL Java_mapnik_VectorTile_queryImpl(JNIEnv* env, jobject obj, jdouble longitude,
                                                           jdouble latitude, jdouble tolerance, jstring layerj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    auto ptr = mapnik::vector_tile_impl::merc_tile_ptr(tile, [](auto&&) {});
    std::string layer_name = layerj != NULL ? JNIString(env, layerj).get() : "";
    auto result = _query(ptr, longitude, latitude, tolerance, layer_name);
    return _queryResultToJNI(env, result);
    TRAILER(NULL);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    queryManyImpl
 * Signature: ([[DLjava/lang/String;D[Ljava/lang/String;)Lmapnik/VectorTile/QueryManyResult;
 */
JNIEXPORT jobject JNICALL Java_mapnik_VectorTile_queryManyImpl(JNIEnv* env, jobject obj, jobjectArray array,
                                                               jstring layerj, jdouble tolerance,
                                                               jobjectArray fieldsj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    auto ptr = mapnik::vector_tile_impl::merc_tile_ptr(tile, [](auto&&) {});
    std::vector<query_lonlat> query;
    {
        if (array == NULL) throw std::invalid_argument("queryMany array cannot be null");
        jsize len = env->GetArrayLength(array);
        query.reserve(len);
        for (jsize i = 0; i < len; ++i) {
            JNIObject obj(env, env->GetObjectArrayElement(array, i));
            JNIDoubleArrayElements lonlat(env, (jdoubleArray)obj.get());
            if (lonlat.size() != 2)
                throw std::invalid_argument("queryMany array element must be of length 2, which stands for [lon,lat]");
            query.push_back({lonlat[0], lonlat[1]});
        }
    }
    std::string layer_name = JNIString(env, layerj).get();
    std::vector<std::string> fields;
    if (fieldsj != NULL) {
        jsize len = env->GetArrayLength(fieldsj);
        fields.reserve(len);
        for (jsize i = 0; i < len; ++i) {
            JNIObject fieldj(env, env->GetObjectArrayElement(fieldsj, i));
            JNIString field(env, (jstring)fieldj.get());
            fields.emplace_back(field.get());
        }
    }
    queryMany_result result;
    _queryMany(result, ptr, query, tolerance, layer_name, fields);
    return _queryManyResultToJNI(env, result);
    TRAILER(NULL);
}