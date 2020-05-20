/**
 * The c++ part of the exam
 */

const float width  = 1000;
const float height = 1000;

float cos(float a);
float sin(float a);
float acos(float d);
float atan2(float y, float x);

void disk(float x, float y);

// NOTE: I assumed positive y is down

// a)
void draw(float t) {
	float radius     = 200.0f;
	float orbit_time = 3.0f;
	float pi         = 3.1415f;

	// Center
	float cx = width / 2.0f;
	float cy = height / 2.0f;

	// Derive angle from time and duration of the rotation
	float _t    = t / orbit_time - (int)t / (int)orbit_time;
	float angle = -2.0f * pi * _t;

	// Position
	float px = radius * cos(angle);
	float py = radius * sin(angle);

	disk(px + cx, py + cy);
}

// b)
void draw(float t) {
	float radius     = 200.0f;
	float orbit_time = 3.0f;
	float pi         = 3.1415f;

	// Center
	float cx = width / 2.0f;
	float cy = height / 2.0f;

	// I chose to use smoothstep interpolation to achieve the effect
	float _t         = t / orbit_time - (int)t / (int)orbit_time;
	float smoothstep = _t * _t * (3.0f - 2.0f * _t);
	float p0         = 0.0f;
	float p1         = 2.0f * pi;
	float angle      = (1.0f - smoothstep) * p0 + smoothstep * p1;

	// Position
	float px = radius * cos(angle);
	float py = radius * sin(angle);

	disk(px + cx, py + cy);
}

// c)
void draw(float t) {
	float radius     = 200.0f;
	float pi         = 3.1415f;
	float orbit_time = 2.0f;

	// Scale time
	float _t = t / orbit_time - (int)t / (int)orbit_time;

	// Set control points based on quadrant
	float p0x, p0y, p1x, p1y, p2x, p2y;
	if (_t >= 0.0f) {
		p0x = 200.0f;
		p0y = 0.0f;
		p1x = 200.0f;
		p1y = 200.0f;
		p2x = 0.0f;
		p2y = 200.0f;
	} else if (_t >= 0.5f) {
		p0x = 0.0f;
		p0y = 200.0f;
		p1x = -200.0f;
		p1y = 200.0f;
		p2x = -200.0f;
		p2y = 0.0f;
	} else if (_t >= 1.0f) {
		p0x = -200.0f;
		p0y = 0.0f;
		p1x = -200.0f;
		p1y = -200.0f;
		p2x = 0.0f;
		p2y = -200.0f;
	} else {
		p0x = 0.0f;
		p0y = -200.0f;
		p1x = 200.0f;
		p1y = -200.0f;
		p2x = 200.0f;
		p2y = 0.0f;
	}

	// Quadratic Bezier Curve
	float px = (1.0f - _t) * (1.0f - _t) * p0x + 2.0f * _t * (1.0f - _t) * p1x +
	           _t * _t * p2x;
	float py = (1.0f - _t) * (1.0f - _t) * p0y + 2.0f * _t * (1.0f - _t) * p1y +
	           _t * _t * p2y;

	disk(px, py);
}

// d)
void draw(float t) {
	float radius     = 200.0f;
	float pi         = 3.1415f;
	float orbit_time = 2.0f;

	// Scale time
	float _t = t / orbit_time - (int)t / (int)orbit_time;

	// Determine quadrant based on cos/sin
	// Does not work!
	float angle = 2.0f * pi * _t;
	// Set angle to start of quadrant
	float _angle =
	    angle -
	    2.0f * pi * (t / orbit_time / 4.0f - (int)t / (int)(orbit_time * 4.0f));
	float xs = cos(_angle);
	float ys = sin(_angle);

	// Set control points based on cos/sin
	float p0x, p0y, p1x, p1y, p2x, p2y;
	p0x = radius * xs;
	p0y = radius * ys;
	p1x = radius * xs;
	p1y = radius * ys;
	p2x = radius * xs;
	p2y = radius * ys;

	// Quadratic Bezier Curve
	float px = (1.0f - _t) * (1.0f - _t) * p0x + 2.0f * _t * (1.0f - _t) * p1x +
	           _t * _t * p2x;
	float py = (1.0f - _t) * (1.0f - _t) * p0y + 2.0f * _t * (1.0f - _t) * p1y +
	           _t * _t * p2y;

	disk(px, py);
}
