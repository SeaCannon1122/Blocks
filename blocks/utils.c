int floor_double(double _value) {
	return _value > -0.000001 ? (int)_value : (_value == (double)(int)_value ? _value : (int)_value - 1);
}

double clamp(double _value, double _max, double _min) {
	return _value > _max ? _max : (_value < _min ? _min : _value);
}