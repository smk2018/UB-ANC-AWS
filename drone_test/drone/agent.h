#ifndef agent_h
#define agent_h

#include <stdio.h>
#include <math.h>
#include <vector>

using namespace std;

class agent{

public:
    
    bool cmp(int a, int b);// compare function
    bool Flag();// flag function to determine flag value
    int double_equals(double const a,double const b); //equal function
    int insect(double x1, double y1, double radius1, double x2, double y2, double radius2); //calculate two circles' insection points
    double distance_sqr(double x1, double y1, double x2, double y2); //calculate distance sqrt function
    double distance(double x1, double y1, double x2, double y2); //calculate distance function
    double get_x_cur();//collect x current coordinate information
    double get_y_cur();//collect y current coordinate information
    double get_x_tag();//collect x tagert coordinate information
    double get_y_tag();//collect y target coordinate information
    void angle_change(double &array1, double &array2); //Trigonometric function and Inverse trigonometric function in matlab and C++ do not use or generate degree from 0 to 360. It from -180 to 180 and depends on quadrants in the Cartesian coordinate system. So i use this function to adjust the angles.
    void array_select(int i ,int n, double array[5][5][5][20]); //select exatly column the agent needs in the Dis[p][i][j][k], i is agent index, n is neighbor index
    void random_tag(int i); //generate random agent's target position
    void random_ori(int i); // generate random agent's original position
    void set_Location(double x1, double y1, double x2, double y2, int j);//each agent will store other agent's information
    void store_location(int i, int num);//only in the first round store_location will input random generate coordinate to each agent
    void weight(int agent); //calculate and store weights between n agents
    void LMST(int agent); //Minimum Spanning Tree (prim algorithm).
    void neighbor_pair_index(int agent); //search and store each agent's neighbor and it's index
    void Distance(int agent); //during round calculation, it calculates and stores distance data(center point to target position)
    void agent_navigation(int agent); //generate each agent's next round center point position
    void navigation_check(int agent, int id_num); //calculate distance between each agent and update new center point
    
    void agent_calculation(int num, int id_num);
    void navigation_check2(int agent, int id_num);
    agent(); //constructor
    ~agent(); //destructor
    
private:

    double x_target, y_target, x_current, y_current; // target position and current position
    double map[40][40] = {0}; // store all links' weight result
    double angle_min1, angle_max1, angle_min2, angle_max2;
    double intersect_x1, intersect_y1, intersect_x2, intersect_y2;//insection point's coordinate
    double min_value, position;
    double theta1[5][5][20] = {0},theta2[5][5][20] = {0};
    double Dis[5][5][5][20];//Distance[i][i][j][k]
    vector<double> x_next, y_next; // next round position
    vector<double> x_cur, y_cur, x_tag, y_tag;//vectors to store all agents' information
    vector<vector<pair<double,double>>> Agent;//store all agent-neighbor index
    vector<pair<double,double>> single;//store agent-neighbor index
    int n_index[10], m_index[10]; //neighbor pair index
};

#endif
