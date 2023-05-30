#include "globals.hpp"
/// -- Geometry class
/*
 * Class:     mapnik_Geometry
 * Method:    getType
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_Geometry_getType(JNIEnv *env, jobject gobj) {
    PREAMBLE;
    mapnik::geometry::geometry<double> *g = LOAD_GEOMETRY_POINTER(gobj);
    return mapnik::geometry::geometry_type(*g);
    TRAILER(0);
}

struct VertexCollector {
    std::vector<std::tuple<unsigned, double, double>> vertices;

    template <typename T> void operator()(const T &vertex_adapter) {
        unsigned command;
        double x, y;
        while ((command = vertex_adapter.vertex(&x, &y)) != mapnik::SEG_END)
            vertices.push_back(std::make_tuple(command, x, y));
    }
};

/*
 * Class:     mapnik_Geometry
 * Method:    getNumPoints
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_Geometry_getNumPoints(JNIEnv *env, jobject gobj) {
    PREAMBLE;
    mapnik::geometry::geometry<double> *g = LOAD_GEOMETRY_POINTER(gobj);

    VertexCollector vertex_collector;
    mapnik::geometry::vertex_processor<VertexCollector> processor(vertex_collector);
    processor(*g);

    return vertex_collector.vertices.size();
    TRAILER(0);
}

/*
 * Class:     mapnik_Geometry
 * Method:    getVertex
 * Signature: (ILmapnik/Coord;)I
 */
JNIEXPORT jint JNICALL Java_mapnik_Geometry_getVertex(JNIEnv *env, jobject gobj, jint pos, jobject coord) {
    PREAMBLE;
    mapnik::geometry::geometry<double> *g = LOAD_GEOMETRY_POINTER(gobj);

    VertexCollector vertex_collector;
    mapnik::geometry::vertex_processor<VertexCollector> processor(vertex_collector);
    processor(*g);
    const std::tuple<unsigned, double, double> &command_and_coords = vertex_collector.vertices.at(pos);

    if (coord) {
        env->SetDoubleField(coord, FIELD_COORD_X, std::get<1>(command_and_coords));
        env->SetDoubleField(coord, FIELD_COORD_Y, std::get<2>(command_and_coords));
    }

    return std::get<0>(command_and_coords);
    TRAILER(0);
}

/*
 * Class:     mapnik_Geometry
 * Method:    getEnvelope
 * Signature: ()Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Geometry_getEnvelope(JNIEnv *env, jobject gobj) {
    PREAMBLE;
    mapnik::geometry::geometry<double> *g = LOAD_GEOMETRY_POINTER(gobj);
    return box2dFromNative(env, mapnik::geometry::envelope(*g));
    TRAILER(0);
}
