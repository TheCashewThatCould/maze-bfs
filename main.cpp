#include <iostream>
#include <math.h>
#include <map>
#include <algorithm>
#include <iterator>
#include <set>
#include <vector>
#include <deque>
#include <iostream>
#include <stack>
#include <string>
#include<sstream>
#include <fstream>
#include <time.h>
#define INF 1000000000000007

using namespace std;
typedef struct cell{
    vector<char> wall;
    bool visited,start,finish,path;
    cell(){
        wall={'1','1','1','1'};
        visited=0;
        start=0;
        finish=0;
        path=0;
    }
}cell;
void connect_adj(vector<vector<cell>> &grid,pair<int,int> a,pair<int,int> b,char val){
    if(a.first-b.first>0){
        grid[a.first][a.second].wall[3]=val;
        grid[b.first][b.second].wall[1]=val;

    }
    else if(a.first-b.first<0){
        grid[a.first][a.second].wall[1]=val;
        grid[b.first][b.second].wall[3]=val;
    }
    else if(a.second-b.second>0){
        grid[a.first][a.second].wall[2]=val;
        grid[b.first][b.second].wall[0]=val;
    }
    else{
        grid[a.first][a.second].wall[0]=val;
        grid[b.first][b.second].wall[2]=val;
    }
}
void adj(vector<vector<cell>> &grid,stack<pair<int,int>> &check,pair<int,int> current){
    int x=current.first;
    int y=current.second;
    grid[x][y].visited=1;
    vector<pair<int,int>> avalible;
    for(int i=-1;i<2;i++){
        if(i+x<grid.size()&&i+x>=0){
            for(int f=-1;f<2;f++){
                if(i!=f&&-1*i!=f&&f+y<grid[0].size()&&f+y>=0&&grid[x+i][y+f].visited==0){
                    avalible.push_back(make_pair(x+i,y+f));
                }
            }
        }
    }
    if(avalible.size()==0) {
        check.pop();
        return;
    }
    pair<int,int> connect=avalible[rand()%avalible.size()];
    check.push(connect);
    connect_adj(grid,connect,current,'0');
}
vector<vector<cell>> gen(int n,pair<int,int> *start,pair<int,int> *finish){
    vector<vector<cell>> res(n,vector<cell> (n,cell()));

    *start=make_pair(rand()%n,rand()%n);
    res[start->first][start->second].start=1;
    *finish = make_pair(rand()%n,rand()%n);

    while(finish->first==start->first&&finish->second==start->second){

        finish->first=rand()%n;
        finish->second=rand()%n;
        cout<<finish->first<<" "<<finish->second<<endl;
    }

    res[finish->first][finish->second].finish=1;
    stack<pair<int,int>> check;
    check.push(*start);
    while(check.size()>0){
        pair<int,int> current=check.top();
        adj(res,check,current);
    }
    return res;
}
void print(vector<vector<cell>> grid,ofstream &file){
    vector<string> res (2*grid.size(),"");
    for(int i=0;i<grid.size();i++){
        res[i*2]+='1';
        res[i*2+1]+='1';
        for(int f=0;f<grid[0].size();f++){
            if(grid[i][f].start){
                res[i*2]+='S';
            }
            else if(grid[i][f].finish){
                res[i*2]+='E';
            }
            else if(grid[i][f].path) res[i*2]+='P';
            else res[i*2]+='0';
            res[i*2]+=grid[i][f].wall[0];
            res[i*2+1]+=grid[i][f].wall[1];
            res[i*2+1]+='1';
        }
        res[i*2]+='1';
        res[i*2+1]+='1';
    }
    for(int i=0;i<=grid.size()*2+1;i++) file<<"1 ";
    file<<endl;
    for(auto i:res){
        for(auto f:i) file<<f<<" ";
        file<<endl;
    }
}
void print_iteration(vector<vector<cell>> grid,vector<vector<int>> distance,ofstream &file){

    vector<string> res (2*grid.size(),"");
    for(int i=0;i<grid.size();i++){
        res[i*2]+='1';
        res[i*2+1]+='1';
        for(int f=0;f<grid[0].size();f++){
            if(grid[i][f].start){
                res[i*2]+='S';
            }
            else if(grid[i][f].finish){
                res[i*2]+='E';
            }
            else if(distance[i][f]!=INT_MAX) {
                    res[i*2]+='P';
            }
            else res[i*2]+='0';
            res[i*2]+=grid[i][f].wall[0];
            res[i*2+1]+=grid[i][f].wall[1];
            res[i*2+1]+='1';
        }
        res[i*2]+='1';
        res[i*2+1]+='1';
    }
    for(int i=0;i<=grid.size()*2+1;i++) file<<"1 ";
    file<<endl;
    for(auto i:res){
        for(auto f:i) file<<f<<" ";
        file<<endl;
    }
}
pair<int,int> check_adjacent(vector<vector<cell>> &grid,pair<int,int> current,deque<pair<int,int>> &to_check,vector<vector<int>> &dis_val){
    int x=current.first;
    int y=current.second;
    vector<pair<int,int>> avalible;
    for(int i=-1;i<2;i++){
        if(i+x<grid.size()&&i+x>=0){
            for(int f=-1;f<2;f++){
                bool if_wall=0;
                if(i==-1&&grid[x][y].wall[3]=='1') if_wall=1;
                else if(i==1&&grid[x][y].wall[1]=='1') if_wall=1;
                else if(f==1&&grid[x][y].wall[0]=='1') if_wall=1;
                else if(f==-1&&grid[x][y].wall[2]=='1') if_wall=1;
                if(!if_wall&&i!=f&&-1*i!=f&&f+y<grid[0].size()&&f+y>=0&&dis_val[x+i][y+f]==INT_MAX){
                    avalible.push_back(make_pair(x+i,y+f));
                    dis_val[x+i][y+f]=dis_val[x][y]+1;
                }
                else if(!if_wall&&i!=f&&-1*i!=f&&f+y<grid[0].size()&&f+y>=0){
                    dis_val[x+i][y+f]=min(dis_val[x][y]+1,dis_val[x+i][y+f]);
                }
            }
        }
    }
    pair<int,int> res=make_pair(-1,-1);
    for(auto i:avalible) {
        if(grid[i.first][i.second].finish) res=i;
        to_check.push_back(i);
    }
    return res;
}
void back_track(vector<vector<cell>> &grid,vector<vector<int>> dis_val,pair<int,int> start){
    if(dis_val[start.first][start.second]==0) return;
    int lowest_val=INT_MAX;
    int x=start.first;
    int y=start.second;
    grid[x][y].path=1;

    pair<int,int> lowest_pos;
    for(int i=-1;i<2;i++){
        if(i+x<grid.size()&&i+x>=0){
            for(int f=-1;f<2;f++){
                if(i!=f&&-1*i!=f&&f+y<grid[0].size()&&f+y>=0&&dis_val[x+i][y+f]<lowest_val){
                    lowest_val=dis_val[x+i][y+f];
                    lowest_pos=make_pair(x+i,y+f);
                }
            }
        }
    }
    connect_adj(grid,lowest_pos,start,'P');
    back_track(grid,dis_val,lowest_pos);
}
void solve(vector<vector<cell>> &grid,pair<int,int> start,pair<int,int> finish){
    vector<vector<int>> dis_val(grid.size(),vector<int> (grid[0].size(),INT_MAX));
    dis_val[start.first][start.second]=0;
    deque<pair<int,int>> to_check;
    to_check.push_back(start);
    int iteration = 0;
    int distance = 1;
    while(to_check.size()>0){
        pair<int,int> current=to_check.front();
        if(dis_val[current.first][current.second]>distance){
            distance++;
            string name = "maze"+to_string(iteration)+".txt";
            iteration++;
            ofstream cur_file("maze_iterations/"+name);
            print_iteration(grid,dis_val,cur_file);
            cur_file.close();
        }
        to_check.pop_front();
        current=check_adjacent(grid,current,to_check,dis_val);
        if(current.first!=-1) break;



    }
    back_track(grid,dis_val,finish);
}
int main()
{
    srand (time(NULL));
    int n;
    cin>>n;
    pair<int,int> start;
    pair<int,int> finish;
    vector<vector<cell>> grid=gen(n,&start,&finish);
    ofstream maze;
    maze.open("maze_iterations/maze.txt");
    print(grid,maze);
    maze.close();
    solve(grid,start,finish);
    //print(grid);
    return 0;
}
