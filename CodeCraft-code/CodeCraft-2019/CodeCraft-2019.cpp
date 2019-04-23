#include "iostream"
#include <string>
#include <vector>
#include<fstream>
#include<ostream>

#include"car.h"
#include"road.h"
#include"cross.h"
#include"graph.h"
#include"planing.h"
using namespace std;

std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;
	int size = str.size();

	for (int i = 0; i<size; i++)
	{
		pos = str.find(pattern, i);
		if (pos<size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}


void output_to_answer(vector<planing> road_plan_vec,string answerPath)
{
	ofstream write_file;
	write_file.open(answerPath);
	// cout << road_plan_vec.size() << endl;
	for (int plan_car = 0; plan_car < road_plan_vec.size(); plan_car++)
	{
		write_file << '(';
		write_file << road_plan_vec[plan_car]._car_id;
		write_file << ", ";
		write_file << road_plan_vec[plan_car]._start_time;
		write_file << ", ";
		vector<int > print_road_vec = road_plan_vec[plan_car]._road_vec;
		// cout << "write road begin" << print_road_vec.size() << endl;
		int i = 0;
		for (; i < print_road_vec.size()-1; i++)
		{
			write_file << print_road_vec[i] << ", ";
		}
		write_file << print_road_vec[i];

		write_file << ')' << endl;
		// cout << plan_car << endl;
	}
	write_file.close();
}

int main(int argc, char *argv[])
{
    std::cout << "Begin" << std::endl;
	
	if(argc < 6){
		std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
		exit(1);
	}
	
	std::string carPath(argv[1]);
	std::string roadPath(argv[2]);
	std::string crossPath(argv[3]);
	std::string presetAnswerPath(argv[4]);
	std::string answerPath(argv[5]);
	
	std::cout << "carPath is " << carPath << std::endl;
	std::cout << "roadPath is " << roadPath << std::endl;
	std::cout << "crossPath is " << crossPath << std::endl;
	std::cout << "presetAnswerPath is " << presetAnswerPath << std::endl;
    std::cout << "answerPath is " << answerPath << std::endl;
	

	vector<car > car_vec;
	vector<cross> cross_vec;
	vector<road> road_vec;
	vector<planing> plan_vec;
	vector<car> priority_car_vec;
	vector<planing> preset_plan_vec;
	
	int max_preset_start_time=0;
	vector<int> graph_index;//vector_index<->crossID
	vector<vector<int>> P;
	vector<vector<int>> D;
	int priority_car_num=0;
	int preset_car_num=0;
	//int cars_per_slot_array[2000];
	//Pathmatirx P;
	//ShortPathTable D;



	/*read the preset answer*/
	ifstream reader;
	reader.open(presetAnswerPath);
	string temp;
	string preset_plan_string;
	getline(reader, temp);
	while (getline(reader, temp))
	{
		preset_plan_string = temp.substr(1, temp.length() - 2);
		std::vector<std::string> result_plan = split(preset_plan_string,",");
		int preset_car_id = atoi(result_plan[0].c_str());
		int preset_start_time = atoi(result_plan[1].c_str());
		if (preset_start_time > max_preset_start_time)
		{
			max_preset_start_time = preset_start_time;
		}
		planing preset_plan = planing(preset_car_id);
		preset_plan._start_time = atoi(result_plan[1].c_str());
		for (int i = 2; i <result_plan.size() ; i++)
		{
			int temp_road = atoi(result_plan[i].c_str());
			preset_plan._road_vec.push_back(temp_road);
		}
		preset_plan_vec.push_back(preset_plan);
	}
	reader.close();
	cout<<"preset_Car_num:"<<preset_plan_vec.size()<<endl;
	cout<<"max_preset_start_time:"<<max_preset_start_time<<endl;

	//vector<int> preset_cars_in_per_slot_vec(max_preset_start_time+1);//index 1-max_preset_start_time reflect the time 1-max_preset_start_time,
	vector<int> preset_cars_in_per_slot_vec(4000);
  	// for (int preset_index = 0; preset_index < max_preset_start_time; preset_index++ )
  	// {
  	// 	preset_cars_in_per_slot_vec.push_back(0);
  	// }
  	for (int preset_plan_index = 0; preset_plan_index < preset_plan_vec.size(); preset_plan_index++ )
  	{
  		preset_cars_in_per_slot_vec[preset_plan_vec[preset_plan_index]._start_time]++;//index 1-max_preset_start_time reflect the time 1-max_preset_start_time,0 was unused

  	}
  	// for (int i = 0; i < preset_cars_in_per_slot_vec.size(); i++)
  	// {
  	// 	cout<<"slot "<<i<<" have:"<< preset_cars_in_per_slot_vec[i]<<endl;
  	// }

	reader.open(carPath);
	string car_string;
	getline(reader, temp);
	while (getline(reader, temp))
	{
		car_string = temp.substr(1, temp.length() - 2);
		std::vector<std::string> result = split(car_string,",");
		int car_id = atoi(result[0].c_str());
		int cross_from = atoi(result[1].c_str());
		int cross_to = atoi(result[2].c_str());
		int car_speed = atoi(result[3].c_str());
		int car_plantime = atoi(result[4].c_str());
		int priority = atoi(result[5].c_str());
		int preset = atoi(result[6].c_str());
		car car_temp = car(car_id, cross_from, cross_to, car_speed, car_plantime, priority,preset);

		if (priority == 1)
		{	
			if (priority_car_num==0)
			car_vec.insert(car_vec.begin(),car_temp);
			else
			{	
				for (unsigned int car_index = 0; car_index < priority_car_num; car_index++)
				{	if (car_temp._car_plantime <= car_vec[car_index]._car_plantime)
					{
							car_vec.insert(car_vec.begin() + car_index , car_temp);
							break;
					}
					else if(car_index == priority_car_num-1)
					{
							car_vec.insert(car_vec.begin() + priority_car_num, car_temp);
							break;
					}
						
				}
					
			}	
		}
		else
		{
			car_vec.push_back(car_temp);
			// if (car_vec.size()==0)
			// 	car_vec.push_back(car_temp);
			// else
			// {
			// 	for (unsigned int car_index = priority_car_num; car_index < car_vec.size(); car_index++)
			// 	{
			// 		if (car_temp._car_speed > car_vec[car_index]._car_speed)
			// 		{
			// 				car_vec.insert(car_vec.begin() + car_index , car_temp);
			// 				break;
			// 		}
					
			// 		else if ((car_index != car_vec.size() - 1) && (car_temp._car_speed > car_vec[car_index+1]._car_speed))
			// 		{
			// 			car_vec.insert(car_vec.begin() + car_index+1, car_temp);
			// 			break;
			// 		}
			// 		else if(car_index == car_vec.size()-1)
			// 		{
			// 			car_vec.push_back(car_temp);
			// 			break;
			// 		}
			// 	}
			// }

		}
		// if (priority == 1)
		// {
		// 	car_vec.insert(car_vec.begin(),car_temp);
		// }
		// else
		// {
		// 	car_vec.push_back(car_temp);
		// }
		if(priority==1)
			priority_car_num++;
		if(preset ==1)
		{
			preset_car_num++;
		}
	//	cout << result[0] << result[1] << result[2] << result[3] << result[4] << result[5] << result[6] << endl;
	//	cout << car_temp.get_speed() << endl;
	}
	 // for (unsigned int car_index = 0; car_index < 1000; car_index++)
	 // {
	 // 	cout << "priority:"<< car_vec[car_index]._priority<<"plantime:"<<car_vec[car_index]._car_plantime<<endl;
	 // }
	// for (int i = 0; i < priority_car_num; ++i)
	// {
	// 	cout<<"plantime:"<<car_vec[i]._car_plantime<<endl;
	// }
	cout << "car_vec.size()" << car_vec.size()<<endl;
	cout << "priority_car_num: " << priority_car_num<<endl;
	cout << "preset_car_num: " << preset_car_num<<endl;
	reader.close();

	reader.open(crossPath);
	string cross_string;
	getline(reader, temp);
	while (getline(reader, temp))
	{
		cross_string = temp.substr(1, temp.length() - 2);
		std::vector<std::string> result_cross = split(cross_string, ",");
		int cross_id = atoi(result_cross[0].c_str());
		int road_one = atoi(result_cross[1].c_str());
		int road_two = atoi(result_cross[2].c_str());
		int road_three = atoi(result_cross[3].c_str());
		int road_four = atoi(result_cross[4].c_str());
		cross cross_temp = cross(cross_id,road_one,road_two,road_three,road_four);
		cross_vec.push_back(cross_temp);
		// cout << result_cross[0] << result_cross[1] << result_cross[2] << result_cross[3] << result_cross[4] << endl;

	}
		graph_index.resize(cross_vec.size());
	for (int i = 0; i < cross_vec.size(); i++)
	{
		graph_index[i] = cross_vec[i]._cross_id;
	}
	reader.close();
	cout << "cross_vec.size()" << cross_vec.size()<<endl;
	
	reader.open(roadPath);
	string road_string;
	getline(reader, temp);
	while (getline(reader, temp))
	{
		road_string = temp.substr(1, temp.length() - 2);
		std::vector<std::string> result_road = split(road_string, ",");
		int road_id = atoi(result_road[0].c_str());
		int road_length = atoi(result_road[1].c_str());
		int road_speed = atoi(result_road[2].c_str());
		int road_channel = atoi(result_road[3].c_str());
		int from_cross = atoi(result_road[4].c_str());
		int to_cross = atoi(result_road[5].c_str());
		int isduplex = atoi(result_road[6].c_str());
		road road_temp = road(road_id,road_length,road_speed,road_channel,from_cross,to_cross,isduplex);
		road_vec.push_back(road_temp);
	//	cout << road_id << " " << road_length << " " << road_speed << " " << road_channel << " " << from_cross << " " << to_cross << " " << isduplex << endl;

	}
	reader.close();
	cout << "road_vec.size()" << road_vec.size()<<endl;


	Graph newgraph = Graph(cross_vec,road_vec);

	newgraph.generate_map_graph(graph_index);

	// for (int i = 0; i < newgraph._cross_num; i++)
	// {
	// 	for (int j = 0; j < newgraph._cross_num; j++)
	// 		cout << newgraph.original_graph_array[i][j]<<" ";
	// 	cout << endl;
	// }


	int counter = 0;
  	int self_max=0;
  	int car_max = 42;
  	int solo_car_index = 1000;
  	int reset_map_index =0;
  	int time_begin;
  	time_begin = max_preset_start_time+5;
  	vector<int> tem;
  	int block_Car_num;
  	int car_counter=0;
  	int car_block=1;
  	int current_time =1;
  	int preset_car_time_index =1 ;//
  	int set_car_num = 0;
	int	set_car_time = 0;
	int car_in_preset_time_i = 0;
	int judeg_flag = 1;

  	for (int tem_i = 0; tem_i < car_vec.size(); tem_i++)
  	{
  		tem.push_back(0);
  	}
  	
	for (int i = 0;  i<car_vec.size() ; i++)
	{
		if(car_vec[i]._preset == 1)
			{continue;}
    	vector<int >route;
    	int index_from = MAX_INT;
    	int index_to = MAX_INT;
	  	for(int find_index = 0; find_index < graph_index.size(); find_index++)// find cross index in graph
	  	{
			if (car_vec[i].get_from_cross() == graph_index[find_index])
				index_from = find_index;
			if (car_vec[i].get_to_cross() == graph_index[find_index])
			  	{index_to = find_index;}
		}
	
		if(counter>2)
	    	counter = 0;
	 	
		string cross_Dijkstra_string = newgraph.Dijkstra(index_from, index_to);
		std::vector<std::string> cross_Dijkstra = split(cross_Dijkstra_string, " ");
		for (unsigned int dij_index = 0; dij_index < cross_Dijkstra.size(); dij_index++)
		{
			int temp_index = atoi(cross_Dijkstra[dij_index].c_str());
			if(dij_index < cross_Dijkstra.size()-1)
				{
					int temp_index_1 = atoi(cross_Dijkstra[dij_index+1].c_str());
					newgraph.graph_array[temp_index][temp_index_1]+=counter;
				}
			route.push_back(graph_index[temp_index]);
		}
		counter++;
		// reset_map_index++;
		// if (reset_map_index>150)
		// {
		// 	newgraph.reset_map();
		// 	reset_map_index =0;
		// }

		planing newplan = planing(car_vec[i]._car_id);
		newplan._car_plan_time = car_vec[i]._car_plantime;
		for (int p = 0; p < route.size() - 1; p++)
		{
			for (int z = 0; z < road_vec.size(); z++)
			{
				
				if (((road_vec[z]._road_from == route[p]) && (road_vec[z]._road_to == route[p+1])) || ((road_vec[z]._road_to == route[p]) && (road_vec[z]._road_from == route[p+1])))
				    {newplan._road_vec.push_back(road_vec[z]._road_id);}
				else
				    {				}
			}
			 
		}
		plan_vec.push_back(newplan);
		// for (int i=0;i<plan_vec.size();i++)
		// {
		// 	cout <<"car_id="<< plan_vec[i]._car_id << "plantime=" << plan_vec[i]._start_time << "planroad";
		// 	for (int j=0;j<plan_vec[i]._road_vec.size();j++)
		// 	{
		// 		cout << plan_vec[i]._road_vec[j] << " ";
		// 	}
		// 	cout << endl;
		// }
	}
			
			int cars_num_insert =41;
			int current_index = 0;
			for (preset_car_time_index; preset_car_time_index <= max_preset_start_time ; ++preset_car_time_index)
			{
				if (preset_cars_in_per_slot_vec[preset_car_time_index] >=cars_num_insert )
				{
					continue;
				}
				if(preset_cars_in_per_slot_vec[preset_car_time_index] <cars_num_insert )
				{

					set_car_num = preset_cars_in_per_slot_vec[preset_car_time_index];
					for (int i = 0; i < cars_num_insert  - set_car_num ; ++i)
					{
						if (current_index <plan_vec.size())
						{


							if (plan_vec[current_index]._car_plan_time >preset_car_time_index)
							{
								//plan_vec[current_index]._start_time = plan_vec[current_index]._car_plan_time +max_preset_start_time;
								if (preset_cars_in_per_slot_vec[plan_vec[current_index]._car_plan_time] >cars_num_insert)
								{
									plan_vec[current_index]._start_time = plan_vec[current_index]._car_plan_time+1;
									preset_cars_in_per_slot_vec[plan_vec[current_index]._start_time]++;
									i--;
								}
								else
								{
									plan_vec[current_index]._start_time = plan_vec[current_index]._car_plan_time;
									preset_cars_in_per_slot_vec[plan_vec[current_index]._start_time]++;
									i--;
								}

							}
							else
							{
								plan_vec[current_index]._start_time = preset_car_time_index;
								preset_cars_in_per_slot_vec[preset_car_time_index]++;
							}
							current_index++;

						}
						else	
							break;
						
					}

				}
				if ((current_index == plan_vec.size()-1) |(preset_car_time_index == max_preset_start_time))
					break;
			}

			for ( current_index; current_index < plan_vec.size() ; ++current_index)
			{
				if (preset_cars_in_per_slot_vec[preset_car_time_index]<54)
				{
					preset_cars_in_per_slot_vec[preset_car_time_index]++;
					plan_vec[current_index]._start_time = preset_car_time_index;
				}
				else
				{
					preset_car_time_index++;
					plan_vec[current_index]._start_time = preset_car_time_index;
					preset_cars_in_per_slot_vec[preset_car_time_index]++;
				}
						
			}
			for (int i = 0; i < preset_car_time_index; ++i)
			{
				cout<<"slot "<<i<<":"<<preset_cars_in_per_slot_vec[i]<<endl;
			}
	output_to_answer(plan_vec,answerPath);
	cout << "plan finished"<<endl;
	return 0;
}