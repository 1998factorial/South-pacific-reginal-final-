#include <iostream>
#include <queue>
#include <vector>
#include <cstring>
using namespace std;

bool vis[105][105];
char grid[105][105];
int N;

struct state{
  int x , y , dis;
  state(){}
  state(int x_ , int y_ , int dis_) : x(x_) , y(y_) , dis(dis_) {}
};

bool check(int i , int j){
  if(i < 1 || i > N || j < 1 || j > N)return false;
  if(grid[i][j] == '#')return false;
  return true;
}

int main(){
  cin >> N;
  for(int i = 1; i <= N; ++i){
    scanf("%s" , grid[i] + 1);
  }
  queue<state> q;
  q.push(state(1 , 1 , 0));
  while(!q.empty()){
    state cur = q.front();
    q.pop();
    if(vis[cur.x][cur.y])continue;
    vis[cur.x][cur.y] = true;
    if(cur.x == N && cur.y == N){
      cout << cur.dis << endl;
      return 0;
    }
    for(int i = cur.y + 1; i <= N; ++i){
      if(!check(cur.x , i))break;
      if(!vis[cur.x][i]){
        q.push(state(cur.x , i , cur.dis + 1));
      }
    }
    for(int i = cur.y - 1; i >= 1; --i){
      if(!check(cur.x , i))break;
      if(!vis[cur.x][i]){
        q.push(state(cur.x , i , cur.dis + 1));
      }
    }
    for(int i = cur.x + 1; i <= N; ++i){
      if(!check(i , cur.y))break;
      if(!vis[i][cur.y]){
        q.push(state(i , cur.y , cur.dis + 1));
      }
    }
    for(int i = cur.x - 1; i >= 1; --i){
      if(!check(i , cur.y))break;
      if(!vis[i][cur.y]){
        q.push(state(i , cur.y , cur.dis + 1));
      }
    }
  }
  return 0;
}
