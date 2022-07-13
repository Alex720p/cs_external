#pragma once


namespace math {
	struct vec2_t {
		float x, y;

		//operator overloading
		vec3_t operator+ (vec3_t& vec) {
			return { this->x + vec.x, this->y + vec.y};
		}
		vec3_t operator- (vec3_t& vec) {
			return { this->x - vec.x, this->y - vec.y};
		}
		vec3_t operator* (float delta) {
			return { this->x * delta, this->y * delta};
		}
	};

	struct vec3_t {
		float x, y, z;


		//operator overloading
		vec3_t operator+ (vec3_t& vec) {
			return { this->x + vec.x, this->y + vec.y, this->z + vec.z };
		}
		vec3_t operator- (vec3_t& vec) {
			return { this->x - vec.x, this->y - vec.y, this->z - vec.z };
		}
		vec3_t operator* (float delta) {
			return { this->x * delta, this->y * delta, this->z * delta };
		}


	};



}