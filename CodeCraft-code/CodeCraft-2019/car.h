class  car
{
public:
	car(int car_id, int car_from_cross, int car_to_cross, int car_speed, int car_plantime,int priority,int preset);
	~ car();
	int get_speed(){ return _car_speed; }
	int get_plantime(){ return _car_plantime; }
	int get_from_cross(){ return _car_from_cross; }
	int get_to_cross(){ return _car_to_cross; }

	int _car_from_cross;
	int _car_to_cross;
	int _car_speed;
	int _car_id;
	int _car_plantime;
	int _priority;
	int _preset;
private:


};

car::car(int car_id, int car_from_cross, int car_to_cross, int car_speed, int car_plantime, int priority, int preset)
{
	_car_id = car_id;
	_car_from_cross = car_from_cross;
	_car_to_cross = car_to_cross;
	_car_speed = car_speed;
	_car_plantime = car_plantime;
	_priority = priority;
	_preset = preset;
	
}

 car::~ car()
{
}