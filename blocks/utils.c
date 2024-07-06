int floor_double(double _double) {
	return _double > -0.000001 ? (int)_double : (_double == (double)(int)_double ? _double : (int)_double - 1);
}