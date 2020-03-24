//// --- Parameters
static void translate_to_mapnik_parameters(JNIEnv *env, jobject javaparams, mapnik::parameters& mapnikparams)
{
	if (!javaparams) return;
	env->CallVoidMethod(javaparams, METHOD_PARAMETERS_COPY_TO_NATIVE, (jlong)(&mapnikparams));
}

class translate_parameter_visitor: public boost::static_visitor<>
{
	JNIEnv *env;
	jobject paramobject;
	jstring key;
public:
	translate_parameter_visitor(JNIEnv* aenv, jobject aparamobject, jstring akey): env(aenv), paramobject(aparamobject), key(akey) {
	}

	void operator()(mapnik::value_integer value) const {
#ifdef BIGINT
		env->CallVoidMethod(paramobject, METHOD_PARAMETERS_SET_LONG, key, (jlong)value);
#else
		env->CallVoidMethod(paramobject, METHOD_PARAMETERS_SET_INT, key, (jint)value);
#endif
	}

	void operator()(mapnik::value_bool value) const {
		env->CallVoidMethod(paramobject, METHOD_PARAMETERS_SET_BOOLEAN, key, (jboolean)value);
	}

	void operator()(mapnik::value_double value) const {
		env->CallVoidMethod(paramobject, METHOD_PARAMETERS_SET_DOUBLE, key, (jdouble)value);
	}

	void operator()(std::string const& value) const {
		env->CallVoidMethod(paramobject, METHOD_PARAMETERS_SET_STRING,
				key,
				env->NewStringUTF(value.c_str()));
	}

	void operator()(mapnik::value_null const& value) const {
		// FIXME
	}

};

/*
 * Class:     mapnik_Parameters
 * Method:    setNativeInt
 * Signature: (JLjava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_mapnik_Parameters_setNativeInt
  (JNIEnv *env, jclass c, jlong ptr, jstring namej, jint value)
{
	PREAMBLE;
	refjavastring name(env, namej);
	mapnik::parameters *params=(mapnik::parameters*)(ptr);
	(*params)[name.stringz]=static_cast<mapnik::value_integer>(value);
	TRAILER_VOID;
}

/*
 * Class:     mapnik_Parameters
 * Method:    setNativeString
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Parameters_setNativeString
  (JNIEnv *env, jclass c, jlong ptr, jstring namej, jstring valuej)
{
	PREAMBLE;
	refjavastring name(env, namej);
	refjavastring value(env, valuej);
	mapnik::parameters *params=(mapnik::parameters*)(ptr);
	(*params)[name.stringz]=value.stringz;
	TRAILER_VOID;
}


/*
 * Class:     mapnik_Parameters
 * Method:    setNativeDouble
 * Signature: (JLjava/lang/String;D)V
 */
JNIEXPORT void JNICALL Java_mapnik_Parameters_setNativeDouble
  (JNIEnv *env, jclass c, jlong ptr, jstring namej, jdouble value)
{
	PREAMBLE;
	refjavastring name(env, namej);
	mapnik::parameters *params=(mapnik::parameters*)(ptr);
	(*params)[name.stringz]=(double)value;
	TRAILER_VOID;
}
