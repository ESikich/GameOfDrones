#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <cmath>

using namespace std;

long int Distance(int x1, int y1, int x2, int y2){
	long int distance_x = (x2 - x1);
	long int distance_y = (y2 - y1);
	long int distance = (sqrt(abs((distance_x * distance_x) - (distance_y * distance_y))));
	return distance;
}

struct GameObject{
	int id, x, y;
	GameObject() :
	id(0),
	x(0),
	y(0){}
};

class Zone: public GameObject{
public:
	Zone();
	int owner;
	vector<int> drone;
	void DroneTotalUpdate(void);
	int total_drones;
};

struct less_than_drones
{
	inline bool operator() (const Zone& zone1, const Zone& zone2)
	{
		return (zone1.total_drones < zone2.total_drones);
	}
};

Zone::Zone(){
	owner = -1;
	total_drones = -1;
}

void Zone::DroneTotalUpdate(void){
	int result = 0;
	for(auto& d : drone){
		result += d;
	}
	total_drones = result;
}

struct ZoneIDLess{
	inline bool operator() (const Zone& zone1, const Zone& zone2){
		return (zone1.id < zone2.id);
	}
};

class ZoneList:public GameObject{
public:
	int val;
	
	bool operator < (const ZoneList& str) const{
		return (val < str.val);
	}
};

class Drone:public GameObject{
public:
	Drone();
	bool busy;
	int player;	
	int target_zone;
	int in_zone;	
	vector<ZoneList> closest_zone;
};

Drone::Drone(){
	busy = false;
	target_zone = -1;
	in_zone = -1;
	player = -1;
}

class Player{
public:
	int id;
	int score;
	vector<int> drone;
	vector<int> zone;
};

void SetTargets(Player& player, vector<Drone>& drone, vector<Zone>& zone, vector<ZoneList>& zone_list){
	//sort(zone.begin(), zone.end(), less_than_drones());
	
	/*for(auto& z : zone_list){
	    z.val = z.val - zone[z.id].drone[player.id];
	    zone[z.id].total_drones = z.val;
	}*/
	
	sort(zone_list.begin(), zone_list.end());
	
	for(auto& d : drone){
		if(d.player == player.id){
			sort(d.closest_zone.begin(), d.closest_zone.end());
			for(auto& z : d.closest_zone){
			    if(d.in_zone == z.id && zone[z.id].total_drones - zone[z.id].drone[player.id] == zone[z.id].drone[player.id] && zone[z.id].owner == player.id){d.busy = true;}
				if(d.busy == false){
					cerr << "Z " << z.id << " total drones " << zone[z.id].total_drones << endl;					
					if(zone[z.id].total_drones < 1 && zone[z.id].owner != player.id && d.in_zone < 0){
						cerr << "1: " << z.id << "t " << zone[z.id].total_drones << endl;
						d.target_zone = z.id;
						d.busy = true;
					}else if(zone[z.id].owner != player.id && zone_list[0].id == z.id && d.in_zone < 0){
						cerr << "2: " << z.id << endl;
						d.target_zone = z.id;
						d.busy == true;
					}else if(d.in_zone < 0 && zone[z.id].owner == player.id && zone[z.id].total_drones - (zone[z.id].total_drones -  zone[z.id].drone[player.id]) != 0 && zone[z.id].drone[player.id] <= zone[z.id].total_drones + 1){
                        d.target_zone = d.closest_zone[0].id;
                        d.busy == true;
					}
				}
			}
		    if(d.target_zone == -1){
		        for(auto& zid : d.closest_zone){
		            if(d.busy == false){
		                if(zone[zid.id].owner != player.id){
		                    d.target_zone = zid.id;
		                    d.busy == true;
		                }
		            }
		        }
		    }if(d.target_zone == -1){d.target_zone = zone_list[3].id;}		
		}
	}
	/*sort(zone.begin(), zone.end(), ZoneIDLess());
		for(auto z : zone ){
		cerr << z.id << " ";
	}*/
}

void ZoneUpdate(vector<Zone>& zone){
	for(auto& z : zone) {
		int zowner;
		cin >> zowner; cin.ignore();
		for(int i = 0; i < zone.size(); ++i){
			z.drone[i] = 0;
		}
		z.owner = zowner;
	}
}

void DroneUpdate(vector<Drone>& drone, vector<Zone>& zone, int player_id, vector<ZoneList>){

	bool debug = false;

	if(debug){cerr << "drone.size():" << drone.size() << " zone.size():" << zone.size() << " player_id:" << player_id << endl;}
	for(auto& d : drone){
		
		/** reset **/ if(debug){cerr << endl << "Reseting drone:" << d.id << endl;} 
		d.in_zone = -1;
		d.x = -1;
		d.y = -1;
		d.busy = false;
		d.target_zone = -1;

		//update values
		int x = -1;
		int y = -1;
		cin >> x >> y; cin.ignore(); if(debug){cerr << " x:" << x << " y:" << y << endl;}
		d.x = x;
		d.y = y;
		
		//check zone relationships
		if(debug){cerr << " Zone Check" << endl;}
		for(auto& z : zone){if(debug){cerr << "  zone.id:" << z.id << endl;}
			
			//check to see if drone is inside of a zone
			if(Distance(d.x, d.y, z.x, z.y) < 10){if(debug){cerr << "  IN_ZONE" << endl;}
				
				//add one to zone player list
				int dplayer = d.player; if(debug){cerr << "    d.player:" << d.player << endl;}
				int dzone = z.drone[dplayer] + 1;
				//add to list of players in zone
				++(z.drone[dplayer]);
				
				//flag the drone's zone
				d.in_zone = z.id;
				
				//if this is the gameplayer's drone check distance to zone
				if(d.player == player_id){if(debug){cerr << "  Distance check d.id:" << d.id << endl;}
					int dist = Distance(d.x, d.y, z.x, z.y); if(debug){cerr << "   dist:" << dist << endl;}
					d.closest_zone[z.id].val = dist;
					d.closest_zone[z.id].id = z.id;
				}
			}
		}
	}
}

int main(){
	
	int player_count = -1;
	int player_id = -1;
	int drone_count = -1;
	int zone_count = -1;
	int total_drone_count = -1;
	
	vector<Player> player;
	vector<Drone> drone;
	vector<Zone> zone;
	vector<ZoneList> least_player_zone;

	/************** Initialization **************/

	//init settings
	cin >> player_count >> player_id >> drone_count >> zone_count; cin.ignore();
	total_drone_count = player_count * drone_count;
	
	//setup vectors
	player.resize(player_count);
	zone.resize(zone_count);
	drone.resize(drone_count * player_count);
	least_player_zone.resize(zone_count);
	
	//init zones
	for(int z = 0; z < zone_count; ++z){
		int x = -1;
		int y = -1;
		zone[z].id = z;
		zone[z].drone.resize(player_count);
		cin >> x >> y; cin.ignore();
		zone[z].x = x;
		zone[z].y = y;
	}

	//init players
	for(int p = 0; p < player_count; ++p){
		player[p].id = p;
		player[p].zone.resize(zone_count);
	}
	
	//init drones
	for(int p = 0; p < player_count; ++p){
		for(int d = 0; d < drone_count; ++d){
			int index = (p * drone_count) + d;
			drone[index].id = index;
			drone[index].player = p;
			drone[index].closest_zone.resize(zone_count);
			for(auto& z : zone){
				int zid = z.id;
				drone[index].closest_zone[zid].id = zid;
				drone[index].closest_zone[zid].x = z.x;
				drone[index].closest_zone[zid].y = z.y;
			}
			player[p].drone.push_back(index);	
		}
	}

	/************** Main Loop ******************/

	int go = 1;
	while(go == 1){
		//get new game info
		ZoneUpdate(zone);
		DroneUpdate(drone, zone, player_id, least_player_zone);
		
		//update drones in zones
		for(auto& z : zone){
			z.DroneTotalUpdate();
		}
		
		for(int z = 0; z < zone_count; ++z){
			least_player_zone[z].id = z;
			least_player_zone[z].val = zone[z].total_drones;
		}
		sort(least_player_zone.begin(), least_player_zone.end());
		
		//update closest zones
		for(auto& d : player[player_id].drone){
			sort(drone[d].closest_zone.begin(), drone[d].closest_zone.end());
		}
		
		//PlayerUpdate(player[player_id]);
		SetTargets(player[player_id], drone, zone, least_player_zone);
		
		//make moves
		for(auto& d : player[player_id].drone ){
			int x = zone[drone[d].target_zone].x;
			int y = zone[drone[d].target_zone].y;
			cout << x << " " << y << endl;
		}
	}
}
