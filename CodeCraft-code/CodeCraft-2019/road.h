
class road
{
public:
	road(int road_id,int road_length,int road_speed,int road_channel,int road_from,int road_to,int isduplex);
	~road();
	int get_road_from(){ return _road_from; }
	int get_road_to(){ return _road_to; }
	int get_road_length(){ return _road_length; }
	int get_duplexflag(){ return _isduplex; }

	int _road_id;
	int _road_length;
	int _road_speed;
	int _road_channel;
	int _road_from;
	int _road_to;
	int _isduplex;
private:

};

road::road(int road_id, int road_length, int road_speed, int road_channel, int road_from, int road_to, int isduplex)
{
	_road_id=road_id;
	_road_length=road_length;
    _road_speed=road_speed;
    _road_channel=road_channel;
    _road_from=road_from;
	_road_to=road_to;
	_isduplex= isduplex;
}

road::~road()
{
}