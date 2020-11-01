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
using namespace std;

agent::agent (void) {//constructor
    cout<<"constructor start!"<<endl;
    x_target = 0;y_target = 0;
    x_current = 0;y_current = 0;
    angle_min1 = 0; angle_max1 = 0;
    angle_min2 = 0; angle_max2 = 0;
    position = 0;
    min_value = 0;
}

agent::~agent (void) {//destructor
    cout<<"destructor finished!"<<endl;
}

bool agent::cmp (int a, int b) { // compare function
    return a<b;
}

int agent::double_equals(double const a, double const b) { //equal function
    static const double ZERO = 1e-9;
    return fabs(a-b) < ZERO;
}

double agent::distance_sqr (double x1, double y1, double x2, double y2) { //calculate distance sqrt function
     return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);  //square of distance between two center points
}

double agent::distance (double x1, double y1, double x2, double y2) { //calculate distance function
    return sqrt(distance_sqr(x1,y1,x2,y2));     //distance between two center points
}

void agent::angle_change (double &array1, double &array2) {//Trigonometric function and Inverse trigonometric function in matlab and C++ do not use or generate degree from 0 to 360. It from -180 to 180 and depends on quadrants in the Cartesian coordinate system. So i use this function to adjust the angles.
    if (array1<0) {//in the different quardant
        if (array2>90) {//in the third quardant
            array2 = 360 - array2;
            array1 = 180 - array1;
        }
        else {//in the fourth quardant
            array2 = 360 - array2;
            array1 = 360 + array1;
        }
    }
    else {// in the second quardant
        if (array2>90) {
            array1 = array2;
        }
    }
}

int agent::insect (double x1, double y1, double radius1, double x2, double y2, double radius2) { //calculate two circles' insection points
    double cos_value[2], sin_value[2];
    double angle_x1,angle_x2,angle_x3,angle_x4;//store x coordinate's angle value
    double angle_y1,angle_y2,angle_y3,angle_y4;//store y coordinate's angle value
    double d1=0, a1=0, b1=0, c1=0, p1=0, q1=0, r1=0;  // x = r1 * cosθ + x1, y = r1 * sinθ + y1
    if (double_equals(x1,x2) && double_equals(y1,y2) && double_equals(radius1,radius2)) {
           return -1;//if two circles have the same center point and radius, return -1
    }
    d1 = distance(x1,y1,x2,y2);  //distance between two center points
    if (d1 > radius1 + radius2 || d1 < fabs(radius1 - radius2)) {
    return 0;//two circles do not have intersection part, return 0
    }
    //input x=r1*cosθ+x1, y=r1*sinθ+y1 into (x-x2)^2+(y-y2)^2 = r2^2 to get insection points
    //(r1*cosθ+x1-x2)^2 + (r1*sinθ+y1-y2)^2=r2^2
    //(r1*cosθ)^2 + (r1*sinθ)^2 + 2*r1*(x1-x2)*cosθ + 2*r1*(y1-y2)*sinθ = r2^2 - (x1-x2)^2 - (y1-y2)^2
    a1 = 2.0 * radius1 * (x1 - x2);//a = 2*r1*(x1-x2)
    b1 = 2.0 * radius1 * (y1 - y2);//b = 2*r1*(y1-y2)
    c1 = radius2 * radius2 - radius1 * radius1 - distance_sqr(x1,y1,x2,y2);//c = r2^2-r1^2-(x1-x2)^2-(y1-y2)^2
    //a*cosθ+b*sinθ = c     sinθ = (1 - (cosθ)^2)^(1 / 2)
    p1 = a1 * a1 + b1 * b1;//p = a^2 + b^2
    q1 = -2.0 * a1 * c1;//q = -2 * a * c r = c^2 - b^2
    //cosθ = (±(q^2 - 4 * p * r)^(1/2) - q) / (2 * p)
    
    if (double_equals(d1, radius1 + radius2) || double_equals(d1, fabs(radius1 - radius2))) {//only have 1 insection point
        cos_value[0] = -q1 / p1 / 2.0;
        sin_value[0] = sqrt(1 - cos_value[0] * cos_value[0]);
        intersect_x1 = radius1 * cos_value[0] + x1;
        intersect_y1 = radius1 * sin_value[0] + y1;

        if (!double_equals(distance_sqr(intersect_x1,intersect_y1,x2,y2), radius2 * radius2)) {
            intersect_y1 = y1 - radius1 * sin_value[0];
        }
               
        angle_x1 = acos(intersect_x1 - x1)*180/Pi;
        angle_y1 = asin(intersect_y1 - y1)*180/Pi;
               
        return 1;//only has one root (x0,y0) between two circles, return 1
    }

    r1 = c1 * c1 - b1 * b1;
    cos_value[0] = (sqrt(q1 * q1 - 4.0 * p1 * r1) - q1) / p1 / 2.0;//cosθ = (±(q^2 - 4 * p * r)^(1/2) - q) / (2 * p)
    cos_value[1] = (-sqrt(q1 * q1 - 4.0 * p1 * r1) - q1) / p1 / 2.0;//cosθ = (±(q^2 - 4 * p * r)^(1/2) - q) / (2 * p)
    sin_value[0] = sqrt(1 - cos_value[0] * cos_value[0]);//sinθ = (1 - (cosθ)^2)^(1 / 2)
    sin_value[1] = sqrt(1 - cos_value[1] * cos_value[1]);//sinθ = (1 - (cosθ)^2)^(1 / 2)
    
    intersect_x1 = radius1 * cos_value[0] + x1;//use radius, center point's x1 and cosθ1 to get insection point's x coordinate
    intersect_x2 = radius1 * cos_value[1] + x1;//use radius, center point's x2 and cosθ2 to get insection point's x coordinate
    intersect_y1 = radius1 * sin_value[0] + y1;//use radius, center point's y1 and sinθ1 to get insection point's y coordinate
    intersect_y2 = radius1 * sin_value[1] + y1;//use radius, center point's y2 and sinθ2 to get insection point's y coordinate

    if (!double_equals(distance_sqr(intersect_x1,intersect_y1,x2,y2), radius2 * radius2)) {//check the root when it in the third quadrant and fourth quadrant
        intersect_y1 = y1 - radius1 * sin_value[0];
    }
    if (!double_equals(distance_sqr(intersect_x2,intersect_y2,x2,y2), radius2 * radius2)) {//check the root when it in the third quadrant and fourth quadrant
        intersect_y2 = y1 - radius1 * sin_value[1];
    }
    if (double_equals(intersect_y1, intersect_y2) && double_equals(intersect_x1, intersect_x2)) {//when it has two the same roots
    if (intersect_y1 > 0) {
            intersect_y2 = -intersect_y2;
        } else {
            intersect_y1 = -intersect_y1;
        }
    }
           
    angle_x1 = acos(intersect_x1 - x1)*180/Pi;//Inverse trigonometric function to get intersection point's angle
    angle_y1 = asin(intersect_y1 - y1)*180/Pi;
    angle_x2 = acos(intersect_x2 - x1)*180/Pi;
    angle_y2 = asin(intersect_y2 - y1)*180/Pi;
    angle_x3 = acos(intersect_x1 - x2)*180/Pi;
    angle_y3 = asin(intersect_y1 - y2)*180/Pi;
    angle_x4 = acos(intersect_x2 - x2)*180/Pi;
    angle_y4 = asin(intersect_y2 - y2)*180/Pi;
    
    angle_change(angle_y1,angle_x1);//transfer angle from (-180~180) to (0~360)
    angle_change(angle_y2,angle_x2);
    angle_change(angle_y3,angle_x3);
    angle_change(angle_y4,angle_x4);
           
    angle_max1 = fmax(fmax(fmax(angle_x1,angle_x2),angle_y1),angle_y2);//select the biggest angle(in the intersection point)
    angle_min1 = fmin(fmin(fmin(angle_x1,angle_x2),angle_y1),angle_y2);//select the smallest angle(in the intersection point)
    angle_max2 = fmax(fmax(fmax(angle_x3,angle_x4),angle_y3),angle_y4);
    angle_min2 = fmin(fmin(fmin(angle_x3,angle_x4),angle_y3),angle_y4);
    
    return 2;//have two intersection points, return 2
}

void agent::array_select (int i, int n, double array[5][5][5][20]) {//select exatly column the agent needs in the Dis[p][i][j][k], i is agent index, n is neighbor index
    int j;
    double d_array[20];
    for (j=0;j<20;j++) {
       d_array[j] = array[i][i][n][j];
     }
    min_value = *min_element(d_array,d_array+20);//record minimum value
    position = min_element(d_array,d_array+20)-d_array;//record minimum value's index
}

void agent::random_tag(int i) { //generate random agent's target position
    double x[5], y[5];
    x[0]=5.1622;
    x[1]=5.1656;
    x[2]=5.6892;
    x[3]=5.2290;
    x[4]=5.5383;
    y[0]=5.7943;
    y[1]=5.6020;
    y[2]=5.7482;
    y[3]=5.9133;
    y[4]=5.9961;
    this->x_target =x[i];
    this->y_target =y[i];
}

void agent::random_ori(int i) { // generate random agent's original position
    double x[5], y[5];
    x[0]=0.3112;
    x[1]=0.2630;
    x[2]=0.4505;
    x[3]=0.1524;
    x[4]=0.0782;
    y[0]=0.5285;
    y[1]=0.6541;
    y[2]=0.0838;
    y[3]=0.8258;
    y[4]=0.4427;
    this->x_current = x[i];
    this->y_current = y[i];
}

bool agent::Flag() { //flag function calculates the distance between current position and target position. Then it return true or false.
    if (sqrt(pow(this->x_current-this->x_target,2)+pow(this->y_current-this->y_target,2)) > 1) {
        return true;
    }
    else {
        return false;
    }
    
}

double agent::get_x_cur() { //collect x current coordinate information
    return this->x_current;
}

double agent::get_y_cur() { //collect y current coordinate information
    return this->y_current;
}

double agent::get_x_tag() { //collect x target coordinate information
    return this->x_target;
}

double agent::get_y_tag() { //collect y target coordinate information
    return this->y_target;
}

void agent::set_Location (double x1, double y1, double x2, double y2, int j) { //collect j agents locate information
    this->x_cur.at(j) = x1;
    this->y_cur.at(j) = y1;
    this->x_tag.at(j) = x2;
    this->y_tag.at(j) = y2;
}

void agent::store_location (int i, int num) { //agent store its' information
    this->x_cur.resize(num);
    this->y_cur.resize(num);
    this->x_tag.resize(num);
    this->y_tag.resize(num);
    this->x_cur.at(i) = this->x_current;
    this->y_cur.at(i) = this->y_current;
    this->x_tag.at(i) = this->x_target;
    this->y_tag.at(i) = this->y_target;
}

void agent::weight (int agent) { //calculate and store weights between n agents
    int i,j;
    double power_x, power_y;
    double weight[10][10];
    double total[10];
    for (i = 0; i < agent; i++) {
        for (j = 0; j < agent; j++) {
           power_x=pow(x_cur.at(i)-x_cur.at(j),2);
           power_y=pow(y_cur.at(i)-y_cur.at(j),2);
           weight[i][j]=100*sqrt(power_x+power_y);//calculate weights between each agent
           //cout<<"weight("<<i<<","<<j<<")="<<weight[i][j]<<" ";
           if (i!=j) {
            this->map[i][j] = weight[i][j];
            this->map[j][i] = weight[i][j];
          }
        }
        total[i] = 0;
        for (j = 0; j< agent; j++) {
            total[i]+=weight[i][j];
        }
    //cout<<"total("<<i<<") = "<<total[i]<<endl;
    }
}

void agent::LMST (int agent) { //Minimum Spanning Tree (prim algorithm). n is agent number
    //cout<<"prim:"<<endl;
    int i=0,j=0,k=0;
    int closest[100];
    double lowcost[100];
    double min;
    bool signal[100];
    signal[0] = true;
    for (i=1;i<agent;i++) { //initialize lowcost and s
        lowcost[i] = this->map[0][i];
        signal[i] = false;
        closest[i] = 0;
    }
    for (i=0;i<(agent-1);i++) { //minimum spanning tree only has n-1 links
        min = 100000;
        k = 0;
        for (j=1;j<agent;j++) { //find minimum link
            if((lowcost[j]<min)&&(!signal[j])) {
                min = lowcost[j];
                k = j;
            }
        }
        //cout<<closest[k]<<","<<k<<" "<<"weight:"<<min<<endl;
        if (closest[k]>k) {
            this->n_index[i] = k; //agent_index & neighbor_index
            this->m_index[i] = closest[k];
        }
        else {
            this->n_index[i] = closest[k];
            this->m_index[i] = k;
        }
        
        signal[k] = true;
        for(j=1;j<agent;j++) {
            if((this->map[k][j]<lowcost[j])&&(!signal[j])) { //if new node to node j has smaller weight, it will replace the original node
                lowcost[j] = this->map[k][j];
                closest[j] = k;
            }
        }
    }
}
void agent::neighbor_pair_index (int agent) { //search and store each agent's neighbor and it's index
    int i,j;
    for (j = 0;j < agent; j++) {
        for (i = 0; i < (agent-1); i++) {
            if (this->n_index[i]==j) {//store each agent's neighbor index
                single.push_back(make_pair(this->n_index[i],this->m_index[i]));
            }
            else if (this->m_index[i]==j) {
                single.push_back(make_pair(this->m_index[i],this->n_index[i]));
            }
        }
        Agent.push_back(single);//update all agents' neighbor index
        single.clear();
     }
}

void agent::Distance (int agent) { //during round calculation, it calculates and stores distance data(center point to target position)
    int i,j,k,p;
    double C_X1[40],C_Y1[40],C_X2[40],C_Y2[40];
    double x1,y1,x2,y2;
    double interval1 = 0,interval2 = 0;
    for (i=0;i<agent;i++) {
        for (j=0;j<agent;j++) {
            if(i!=j) {
                x1 = this->x_cur.at(i);y1 = this->y_cur.at(i);
                x2 = this->x_cur.at(j);y2 = this->y_cur.at(j);
                switch (insect(x1,y1,1,x2,y2,1)) {
                    case -1:
                       cout<<"THE CIRCLES ARE THE SAME"<<endl;
                       break;
                    case 0:
                       cout<<"NO INTERSECTION"<<endl;
                       break;
                    case 1:
                        cout<<"ONLY ONE INTERSECTION POINT"<<endl;
                       //cout<<fixed<<setprecision(3)<<"point("<<i+1<<","<<j+1<<")'s angle1: ("<<angle_min1<<" "<<angle_max1<<")"<<endl;
                       break;
                    case 2:
                       //cout<<fixed<<setprecision(4)<<"point("<<i+1<<","<<j+1<<")'s root1: ("<<intersect_x1<<" "<<intersect_y1<<")"<<endl;
                       //cout<<fixed<<setprecision(4)<<"point("<<i+1<<","<<j+1<<")'s root2: ("<<intersect_x2<<" "<<intersect_y2<<")"<<endl;
                       //cout<<fixed<<setprecision(3)<<"point("<<i+1<<","<<j+1<<")'s angle1: ("<<angle_min1<<" "<<angle_max1<<")"<<endl;
                       //cout<<fixed<<setprecision(3)<<"point("<<i+1<<","<<j+1<<")'s angle2: ("<<angle_min2<<" "<<angle_max2<<")"<<endl;
        
                       if (angle_max1 - angle_min1 < 180) {//select those points which will be next round center point
                           interval1 = (angle_max1 - angle_min1)/9;//select 10 points
                           for(k=0;k<10;k++) {//calculate 10 points' angle
                               theta1[i][j][k] = angle_min1 + k*interval1;
                           }
                       }
                       else {
                           interval1 = (360 + angle_min1 - angle_max1)/9;
                           for(k=0;k<10;k++) {
                               theta1[i][j][k] = angle_max1 + k*interval1;
                           }
                       }
                       for (k=0;k<10;k++) {//use 10 angles to calculate 10 points' coordinate
                           C_X1[k] = this->x_cur.at(i) + 1*cos(theta1[i][j][k]*Pi/180);
                           C_Y1[k] = this->y_cur.at(i) + 1*sin(theta1[i][j][k]*Pi/180);
                           for (p=0;p<agent;p++) {//input all possible results
                               Dis[p][i][j][k] = sqrt(pow(C_X1[k]-this->x_tag.at(p),2)+pow(C_Y1[k]-this->y_tag.at(p),2));
                           }
                       }
                       if (angle_max2 - angle_min2 < 180) {
                           interval2 = (angle_max2 - angle_min2 - 10)/9;
                           for (k=0;k<10;k++) {
                               theta2[i][j][k] = angle_min2 + 5 + k*interval2;
                           }
                       }
                       else {
                           interval2 = (360 + angle_min2 - angle_max2 - 10)/9;
                           for (k=0;k<10;k++) {
                               theta2[i][j][k] = angle_max2 + 5 + k*interval2;
                           }
                       }
                       for (k=0;k<10;k++) {
                           C_X2[k] = this->x_cur.at(j) + 1*cos(theta2[i][j][k]*Pi/180);
                           C_Y2[k] = this->y_cur.at(j) + 1*sin(theta2[i][j][k]*Pi/180);
                           for (p=0;p<agent;p++) {
                               Dis[p][i][j][k+10] = sqrt(pow(C_X2[k]-this->x_tag.at(p),2)+pow(C_Y2[k]-this->y_tag.at(p),2));
                           }
                       }
                  }//switch
                }//if
            } //for j
         }//for i
}

void agent::agent_navigation (int agent) { //generate each agent's next round center point position
    int i,j,n;
    double position1;
    double Pos[10][10] = {0},Min_Value[10][10] = {0};
    double p_x[10] = {0}, p_y[10] = {0}, distance[10] = {0};
    vector<int>id;
    vector<int>::iterator it;
    this->x_next.resize(agent);
    this->y_next.resize(agent);
    position = 0;
    position1 = 0;
    for (i = 0; i < agent; i++) {//input agent's id
        id.push_back(i);
    }
    for (i = 0; i < agent; i++) {
        if (Agent[i].size() > 1) {//if agent i has more than 1 neighbor
            for (j = 0;j < Agent[i].size();j++) {//Agent[i].size() equals to neighbor number
                for (it = id.begin();it < id.end();it++) {//except agent i, test all agents
                    if (*(it) != Agent[i][j].first) {//if neighbor id doesn't equal to i
                        if (Agent[i][j].second == *(it) ) {//find agent i's neighbor id
                            n = *(it);//agent i's neighbor is n
                            array_select(i,n,Dis);//select minimum value and it's index in the Dis[i][i][n][k]
                            Pos[i][j] = position;//minimum value's index
                            Min_Value[i][j] = min_value;//minimum value
                            if (position > 10) {//position > 10 means the selected angle's center point is (Xn,Yn)
                                p_x[j] = this->x_cur.at(n) + 1*cos(theta2[i][n][(int)position-10]*Pi/180);
                                p_y[j] = this->y_cur.at(n) + 1*sin(theta2[i][n][(int)position-10]*Pi/180);
                                distance[j] = sqrt(pow(p_x[j] - this->x_tag.at(i),2) + pow(p_y[j] - this->y_tag.at(i),2));
                            }
                            else {//position < 10 means the selected angle's center point is (Xi,Yi)
                                p_x[j] = this->x_cur.at(i) + 1*cos(theta1[i][n][(int)position]*Pi/180);
                                p_y[j] = this->y_cur.at(i) + 1*sin(theta1[i][n][(int)position]*Pi/180);
                                distance[j] = sqrt(pow(p_x[j] - this->x_tag.at(i),2) + pow(p_y[j] - this->y_tag.at(i),2));
                            }
                        }
                    }
                }
            }
            position1 = min_element(distance,distance+Agent[i].size())-distance;//find better choice in the distance  array
            this->x_next.at(i) = p_x[(int)position1];//update next round's x coordinate
            this->y_next.at(i) = p_y[(int)position1];//update next round's y coordinate
            memset(distance,0,sizeof(distance));//clean the distance array
        }
        else if (Agent[i].size() == 1) { //agent i only has 1 neighbor
            for (it = id.begin();it < id.end();it++) {//except agent i, test all agents
                if (*(it) != Agent[i][0].first) {//if neighbor id doesn't equal to i
                    if (Agent[i][0].second == *(it) ) {//find agent i's neighbor id
                        n = *(it);//agent i's neighbor is n
                        array_select(i,n,Dis);//select minimum value and it's index in the Dis[i][i][n][k]
                        Pos[i][0] = position;//minimum value's index
                        Min_Value[i][0] = min_value;//minimum value
                        if (position > 10) {//position > 10 means the selected angle's center point is (Xn,Yn)
                            p_x[0] = this->x_cur.at(n) + 1*cos(theta2[i][n][(int)position-10]*Pi/180);
                            p_y[0] = this->y_cur.at(n) + 1*sin(theta2[i][n][(int)position-10]*Pi/180);
                            distance[0] = sqrt(pow(p_x[0] - this->x_tag.at(i),2) + pow(p_y[0] - this->y_tag.at(i),2));
                        }
                        else {//position < 10 means the selected angle's center point is (Xi,Yi)
                            p_x[0] = this->x_cur.at(i) + 1*cos(theta1[i][n][(int)position]*Pi/180);
                            p_y[0] = this->y_cur.at(i) + 1*sin(theta1[i][n][(int)position]*Pi/180);
                            distance[0] = sqrt(pow(p_x[0] - this->x_tag.at(i),2) + pow(p_y[0] - this->y_tag.at(i),2));
                        }
                    }
                }
            }
            this->x_next.at(i) = p_x[0];//update next round's x coordinate
            this->y_next.at(i) = p_y[0];//update next round's y coordinate
            memset(distance,0,sizeof(distance));//clean the distance array
        }
    }
}

void agent::navigation_check (int agent, int id_num) { //calculate distance between each agent and update new center point
    int i,j;
    double test[20][20] = {0};
    bool flag = false;
    for(i = 0;i < agent; i++) {
        for(j = 0;j < agent; j++) {
            test[i][j] = sqrt(pow(this->x_next.at(i)-this->x_next.at(j),2)+pow(this->y_next.at(i)-this->y_next.at(j),2));//after updating next round position, test the distance between each agent.
            if(test[i][j] < 1.0) {//if smaller than 1m, move to next round position
                flag = true;
            }
            else {//if bigger than 1m, move to next round position with half distance
                flag = false;
            }
        }
        
        if(flag) {
            this->x_cur.at(i) = this->x_next.at(i);//update new x coordinate
            this->y_cur.at(i) = this->y_next.at(i);//update new y coordinate
        }
        else {
            this->x_cur.at(i) = this->x_cur.at(i) + 0.5*(this->x_next.at(i)-this->x_cur.at(i));//update new x coordinate
            this->y_cur.at(i) = this->y_cur.at(i) + 0.5*(this->y_next.at(i)-this->y_cur.at(i));//update new y coordinate
        }
    }
    this->x_current = x_cur.at(id_num);
    this->y_current = y_cur.at(id_num);
    cout<<fixed<<setprecision(4)<<"x_cur("<<id_num<<")= "<<x_cur.at(id_num)<<" "<<"y_cur("<<id_num<<")= "<<y_cur.at(id_num)<<endl;
}



//Qt_Code
void agent::agent_calculation (int num, int id_num) { //num is agent number, id_num is agent id
    int i;
    double angle[num], x_angle[num], y_angle[num];
    this->x_next.resize(num);
    this->y_next.resize(num);
    for(i = 0; i < num; i++) {
        angle[i] = atan((this->y_tag.at(i) - this->y_cur.at(i)) / (this->x_tag.at(i) - this->x_cur.at(i)))*180/Pi;
        if(x_tag.at(i) - x_cur.at(i) >= 0) {
            if(y_tag.at(i) - y_cur.at(i) >= 0) {        //one
                x_angle[i] = angle[i];
                y_angle[i] = angle[i];
            }
            else {                                      //four
                x_angle[i] = 360 + angle[i];
                y_angle[i] = 360 + angle[i];
            }
        }
        else {
            if(x_tag.at(i) - x_cur.at(i) >= 0) {        //two
                x_angle[i] = 180 + angle[i];
                y_angle[i] = 180 + angle[i];
            }
            else {                                      //three
                x_angle[i] = 180 + angle[i];
                y_angle[i] = 180 + angle[i];
            }
        }
        
        this->x_next.at(i) = this->x_cur.at(i) + 1*cos(x_angle[i]*Pi/180);
        this->y_next.at(i) = this->y_cur.at(i) + 1*sin(y_angle[i]*Pi/180);
    }
}

void agent::navigation_check2 (int num, int id_num) {
    int i,j;
    double test[20][20] = {0};
    bool flag = false;
    for(i = 0;i < num; i++) {
        for(j = 0;j < num; j++) {
            test[i][j] = sqrt(pow(this->x_next.at(i)-this->x_next.at(j),2)+pow(this->y_next.at(i)-this->y_next.at(j),2));//after updating next round position, test the distance between each agent.
            if(test[i][j] < 0.8) {//if smaller than 0.8m, move to next round position
                flag = flag && true;
            }
            else {//if bigger than 0.8m, move to next round position with half distance
                flag = flag && false;
            }
        }
        
        if(flag) {
            this->x_cur.at(i) = this->x_next.at(i);//update new x coordinate
            this->y_cur.at(i) = this->y_next.at(i);//update new y coordinate
        }
        else {
            this->x_cur.at(i) = this->x_cur.at(i) + 0.5*(this->x_next.at(i)-this->x_cur.at(i));//update new x coordinate
            this->y_cur.at(i) = this->y_cur.at(i) + 0.5*(this->y_next.at(i)-this->y_cur.at(i));//update new y coordinate
        }
    }
    this->x_current = x_cur.at(id_num);
    this->y_current = y_cur.at(id_num);
    cout<<fixed<<setprecision(4)<<"x_cur("<<id_num<<")= "<<x_cur.at(id_num)<<" "<<"y_cur("<<id_num<<")= "<<y_cur.at(id_num)<<endl;
}
