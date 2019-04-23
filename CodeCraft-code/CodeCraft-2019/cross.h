class cross
{
public:
	cross(int cross_id,
	int road_one,
	int road_two,
	int road_three,
	int road_four);

	~cross();
	int _cross_id;
	int _road_one;
	int _road_two;
	int _road_three;
	int _road_four;

private:

};

cross::cross(int cross_id,int road_one,int road_two,int road_three,int road_four)
{
	_cross_id = cross_id;
	_road_one = road_one;
	_road_two = road_two;
	_road_three = road_three;
	_road_four = road_four;
}

cross::~cross()
{
}