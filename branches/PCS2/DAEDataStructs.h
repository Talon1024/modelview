#include "matrix3d.h"

#ifndef _DAEDataStructs_H_
#define _DAEDataStructs_H_


class DAEInput {
	public:

		DAEInput() {
			valid = false;
		}

		DAEInput(daeURI uri);
		int x_offset();
		int y_offset();
		int z_offset();
		std::vector<float> *values();
		int stride();
		bool is_valid();
		int u_offset();
		int v_offset();
	private:
		int x,y,z, strides,u,v;
		std::vector<float> *value;
		bool valid;
};

class DAEInputs {
public:
	DAEInputs(daeElement *element, std::string doc, DAE *dae);
	int max_offset;
	int pos_offset;
	int norm_offset;
	int uv_offset;
	DAEInput pos, norm, uv;

};

// parses ints into a vector
std::vector<int> *parse_int_array(const char* chars, unsigned int count = -1);

// parses floats into a vector
std::vector<float> *parse_float_array(const char* chars, unsigned int count = -1);

std::string write_int_array(std::vector<int> vec);

std::string write_float_array(std::vector<float> vec);

std::string write_vector3d(vector3d vec,vector3d scale = vector3d(1,1,1));

vector3d relative_to_absolute(vector3d vec, pcs_sobj *subobj, std::vector<pcs_sobj*> *subobjs);

vector3d absolute_to_relative(vector3d vec, pcs_sobj *subobj, std::vector<pcs_sobj*> *subobjs);

std::string int_to_string(int i);

void add_accessor(daeElement *element, std::string source, int count, bool uv = false);

void add_param(daeElement *accessor, std::string name);

std::string strip_texture(std::string path);

vector3d calculate_rotation(vector3d vec, vector3d base);

void write_rotations(daeElement *element, vector3d rotation);

bool vector3d_comparator(vector3d a, vector3d b);

vector3d radius_to_scale(float radius);

void add_scale(daeElement *element, vector3d scale, vector3d parent_scale = vector3d(1,1,1));

std::map<std::string, std::string> *get_texture_mappings(daeElement *element);

#endif