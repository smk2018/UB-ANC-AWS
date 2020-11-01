#include <iostream>
#include <cstring>
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <iomanip>
#include <random>
#include <vector>
#include <ctime>
#include <random>
#include "agent.h"

#define Pi 3.1415

int main() {
    int i = 0, j = 0, k = 0, check = 0;
    bool flag = true;       //flag and Flag() control while loop
    int num = 5;            //num is agent number
    double x1,y1,x2,y2;     //(x1,y1) is current coordinate, (x2,y2) is target coordinate
    vector<agent*> AgentVector; //it generates a vector with multiple agents
    for (i = 0; i < num; i++) { //for loop generates multiple new agents and each agent generates random original coordinate and target coordinate, then call Flag()
        AgentVector.push_back(new agent);
        AgentVector.at(i)->random_ori(i);
        AgentVector.at(i)->random_tag(i);
        AgentVector.at(i)->Flag();
    }
    while (flag) { //while loop will continue working until the distance between current coordinate and target coordinate smaller than 0.8m
        for (i = 0;i < num;i++) {
            flag = flag || (AgentVector.at(i)->Flag()); //it will store all agents flag value.
            
            if (AgentVector.at(i)->Flag() == false) {
                check += 1;
            }
        }
        
        if (check >= 4) {
            break;
        }
        
        for (i = 0;i < num;i++) {//this for loop simulates drone's broadcast process
            if (k == 0) {
                AgentVector.at(i)->store_location(i,num); //only in the first round store_location will input random generate coordinate to each agent
            }
            for (j = 0;j < num;j++) {
                if(i != j) {
                    x1 = AgentVector.at(j)->get_x_cur();//input current x coordinate
                    y1 = AgentVector.at(j)->get_y_cur();//input current y coordinate
                    x2 = AgentVector.at(j)->get_x_tag();//input target x coordinate
                    y2 = AgentVector.at(j)->get_y_tag();//input target x coordinate
                    AgentVector.at(i)->set_Location(x1,y1,x2,y2,j);//each agent will store other agent's information
                }
                else {
                    continue;
                }
            }
        }
        for (i = 0;i < num; i++) {//calculate next position
           //AgentVector.at(i)->weight(num);//weight function to generate all link's weight
           //AgentVector.at(i)->LMST(num);//LMST function to calculate minimum spanning tree with all agents
           //AgentVector.at(i)->neighbor_pair_index(num);//store the agent-neighbor index in the LMST function
           //AgentVector.at(i)->Distance(num);//calculate the distance between current coordinate and target coordinate with Dis[agent][agent][neighbor][k]
           //AgentVector.at(i)->agent_navigation(num);//select the best point to be next round coordinate
           //AgentVector.at(i)->navigation_check(num,i);//check each agent's distance smaller than radius(1m). If so, move to next round position. If not, move to next round position with half distance
            AgentVector.at(i)->agent_calculation(num, i);
            AgentVector.at(i)->navigation_check2(num, i);
        }
        k = k + 1;//round record
    }
    cout<<"mission complete!"<<endl;
    for (i = 0; i < num; i++) {
        delete AgentVector[i];
    }
    return 0;
}

