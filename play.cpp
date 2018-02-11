#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>

using namespace std;


vector<vector<char> > board(11, vector<char>(11));
char temp;
char color;
char player;
char opponent;
int MAX_EXPLORE_DEPTH = 2;
//--------------------------------------------------------------

int do_BFS(char color,int x, int y)
{
    queue< pair< pair<int,int> ,int > > q;
    pair< pair<int,int> , int> node;
    int level = 0;
    int sum=0;
    int i,j;
    board[x][y] = 'D';
    q.push(make_pair(make_pair(x,y),level));
    while(!q.empty())
    {
        node = q.front();
        q.pop();
        /*if(color = 'R')
            sum = sum + (node.second*node.first.second)/(node.second + node.first.second + 1);
        else
            sum = sum + (node.second*node.first.first)/(node.second + node.first.first + 1);*/

        sum+=node.second;
        i=node.first.first;
        j=node.first.second;
        if((j+1)<11 && board[i][j+1] == color)
        {
            board[i][j+1] = 'D';
            q.push(make_pair(make_pair(i,j+1),node.second+1));
        }
        if((j-1)>=0 && board[i][j-1] == color)
        {
            board[i][j-1] = 'D';
            q.push(make_pair(make_pair(i,j-1),node.second+1));
        }
        if((i+1)<11 && board[i+1][j] == color)
        {
            board[i+1][j] = 'D';
            q.push(make_pair(make_pair(i+1,j),node.second+1));
        }
        if((i-1)>=0 && board[i-1][j] == color)
        {
            board[i-1][j] = 'D';
            q.push(make_pair(make_pair(i-1,j),node.second+1));
        }
        if((i-1)>=0 && (j+1)<11 && board[i-1][j+1] == color)
        {
            board[i-1][j+1] = 'D';
            q.push(make_pair(make_pair(i-1,j+1),node.second+1));
        }
        if((i+1)<11 && (j-1)>=0 && board[i+1][j-1] == color)
        {
            board[i+1][j-1] = 'D';
            q.push(make_pair(make_pair(i+1,j-1),node.second+1));
        }

    }
    return sum;
}

int BFS_check(char color)
{
    int val = 0;
    for(int i=0;i<11;i++)
    {
        for(int j=0;j<11;j++)
        {
            if(board[i][j] == color)
                val += do_BFS(color, i, j);
        }
    }
    for(int i=0;i<11;i++)
    {
        for(int j=0;j<11;j++)
        {
            if(board[i][j] == 'D')
                board[i][j] = color;
        }
    }
    return val;
}


int heuristic1(int min_max=1){
    //""" number of connected pieces of the player minus the number of connected pieces of the opponent """
    int player_score = BFS_check(player);
    int opponent_score = BFS_check(opponent);

    if(min_max)
        return player_score-opponent_score;
    else
        return opponent_score-player_score;
	
}

float check_bridge(char player,int x,int y){
    //""" check if there is a bridge pattern"""
    if(board[x][y] != player)
        return 0;

    float bridge_pattern = 0;
    
    float v_score = 1.0;
    float h_score = 1.0;
    if(player == 'B')  // red
        {
            v_score = 2.0;
            h_score = 1.4;
        }
    else  //blue
        h_score = 2.0;

    // check vertical patterns
    if(y <= 8)
    {
        if(x >= 1)
        {
            bridge_pattern += (board[x-1][y+2] == player)*v_score;
            bridge_pattern -= (board[x-1][y+1] == opponent)*v_score*0.5;
            bridge_pattern -= (board[x][y+1] == opponent)*v_score*0.5;
        }
    }
    if(y == 9)
    {
        if(x >= 1)
        {
            bridge_pattern += (board[x-1][y+1] == player)*v_score*0.5;
            bridge_pattern += (board[x-1][y+1] != opponent)*v_score*0.5;
        }
        if(x <= 9)
        {
            bridge_pattern += (board[x][y+1] == player)*v_score*0.5;
            bridge_pattern += (board[x][y+1] != opponent)*v_score*0.5;
        }
    }
    else
        bridge_pattern += v_score*0.5;

    // check horizontal patterns
    if(x <= 8)
    {
        if(y >= 1)
        {
            bridge_pattern += (board[x+2][y-1] == player)*h_score;
            bridge_pattern -= (board[x+1][y-1] == opponent)*h_score*0.5;
            bridge_pattern -= (board[x+1][y] == opponent)*h_score*0.5;
        }
        if(y <= 9)
        {
            bridge_pattern += (board[x+1][y+1] == player)*h_score;
            bridge_pattern -= (board[x+1][y] == opponent)*h_score*0.5;
            bridge_pattern -= (board[x][y+1] == opponent)*h_score*0.5;
        }
    }
    else if(x == 9)
    {
        if(y >= 1)
        {
            bridge_pattern += (board[x+1][y-1] == player)*h_score*0.5;
            bridge_pattern += (board[x+1][y-1] != opponent)*h_score*0.5;
        }
        if(y <= 9)
        {
            bridge_pattern += (board[x+1][y] == player)*h_score*0.5;
            bridge_pattern += (board[x+1][y] != opponent)*h_score*0.5;
        }    
    }
    else
        bridge_pattern += h_score*0.5;

    return bridge_pattern;
}

int heuristic2(int min_max=1){
    //""" calculate scores based on bridge patterns in the board """
    // red: 0
    // blue: 1

    int p_patterns = 0;
    int o_patterns = 0;

    for(int x = 0; x<11;x++)
    {
        for(int y = 0; y<11;y++)
        {
            if( board[x][y] == player )
                p_patterns += check_bridge(player, x, y);
            else if( board[x][y] == opponent )
                o_patterns += check_bridge(opponent, x, y);
            else
                ;
        }
    }
    if(min_max == 1)
        return p_patterns-o_patterns;
    else
        return o_patterns-p_patterns;
}


// Initial values of Aplha and Beta
const int MAX = 1000;
const int MIN = -1000;
 
// Returns optimal value for current player (Initially called
// for root and maximizer)
pair<pair<int, int>,int > minimax(int depth, int nodeIndex, bool maximizingPlayer,
            int alpha, int beta)
{
 
    pair<pair<int,int>,int> res;
    int val;
    int x = rand()%11, y = rand()%11;
    int flag = 0;
    if(player=='R')
    {
        if (maximizingPlayer)
        {
            int best = MIN;

            // Recur for left and right children
            for(int i=0; i<11 ; i++)
            {
                for(int j=0; j<11; j++)
                {
                    if(board[i][j] != 'U')
                        continue;

                    board[i][j] = player;    

                    if(depth + 1 == MAX_EXPLORE_DEPTH)
                    {
                        res = make_pair(make_pair(i,j),heuristic2()+heuristic1());
                    }
                    else res = minimax(depth+1, nodeIndex*2+i,
                                    false,alpha, beta);

                    val = res.second;
                    board[i][j] = 'U';                
                    best = max(best, val);
                    if(best == val)
                    {
                        x = res.first.first;
                        y = res.first.second;
                    }
                    alpha = max(alpha, best);

                    // Alpha Beta Pruning
                    if (beta <= alpha)
                    {
                        flag = 1;
                        break;
                    }
                }
                if(flag)
                    break;
            }
            return make_pair(make_pair(x,y),best);
        }
        else
        {
            int best = MAX;

            // Recur for left and right children
            for (int j=0; j<11; j++)
            {
                for(int i=0; i<11; i++)
                {
                    if(board[i][j] != 'U')
                        continue;

                    board[i][j] = opponent; 
                    if(depth + 1 == MAX_EXPLORE_DEPTH)
                    {
                        res = make_pair(make_pair(i,j),heuristic2(1));
                    }
                    else
                        res = minimax(depth+1, nodeIndex*2+i,true,alpha, beta);

                    val = res.second;
                    board[i][j] = 'U';                
                    best = min(best, val);
                    if(best == val)
                    {
                        x = res.first.first;
                        y = res.first.second;
                    }

                    beta = min(beta, best);

                    // Alpha Beta Pruning
                    if (beta <= alpha)
                    {
                        flag = 1;
                        break;
                    }    
                }
                if(flag)
                    break;
            }
            return make_pair(make_pair(x,y),best);
        }
    }
    else
    {
        if (maximizingPlayer)
        {
            int best = MIN;

            // Recur for left and right children
            for(int j=0; j<11 ; j++)
            {
                for(int i=0; i<11; i++)
                {
                    if(board[i][j] != 'U')
                        continue;

                    board[i][j] = player;    

                    if(depth + 1 == MAX_EXPLORE_DEPTH)
                    {
                        res = make_pair(make_pair(i,j),heuristic2()+heuristic1());
                    }
                    else res = minimax(depth+1, nodeIndex*2+i,
                                    false,alpha, beta);

                    val = res.second;
                    board[i][j] = 'U';                
                    best = max(best, val);
                    if(best == val)
                    {
                        x = res.first.first;
                        y = res.first.second;
                    }
                    alpha = max(alpha, best);

                    // Alpha Beta Pruning
                    if (beta <= alpha)
                    {
                        flag = 1;
                        break;
                    }
                }
                if(flag)
                    break;
            }
            return make_pair(make_pair(x,y),best);
        }
        else
        {
            int best = MAX;

            // Recur for left and right children
            for (int i=0; i<11; i++)
            {
                for(int j=0; j<11; j++)
                {
                    if(board[i][j] != 'U')
                        continue;

                    board[i][j] = opponent; 
                    if(depth + 1 == MAX_EXPLORE_DEPTH)
                    {
                        res = make_pair(make_pair(i,j),heuristic2(1));
                    }
                    else
                        res = minimax(depth+1, nodeIndex*2+i,true,alpha, beta);

                    val = res.second;
                    board[i][j] = 'U';                
                    best = min(best, val);
                    if(best == val)
                    {
                        x = res.first.first;
                        y = res.first.second;
                    }

                    beta = min(beta, best);

                    // Alpha Beta Pruning
                    if (beta <= alpha)
                    {
                        flag = 1;
                        break;
                    }    
                }
                if(flag)
                    break;
            }
            return make_pair(make_pair(x,y),best);
        }
    }
}

// Edit this function with your code.
vector<int> play(vector<vector<char > > board) 
{
    int count = 0;
    for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			if(board[i][j] != 'U')
                count++;
		}
	}
    if(count == 1)
    {
        for (int i = 0; i < 11; i++) {
            for (int j = 0; j < 11; j++) {
                if(board[i][j] != 'U')
                {
                    if(board[i][j] == 'B')
                        break;
                    vector<int> result;
	                result.push_back(i);
	                result.push_back(j);
	                return result;
                }
            }
	    }
    }
	pair<pair<int,int> , int> res = minimax(0,0,true,MIN,MAX);
	int x = res.first.first;
	int y = res.first.second;
	
	vector<int> result;
	result.push_back(x);
	result.push_back(y);
	return result;
}

int main() 
{	
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			cin >> board[i][j];
		}
	}
	cin >> color;

    player = color;
    if(player == 'B')
        opponent = 'R';
    else
        opponent = 'B';
	
	vector<int> xy = play(board);
	cout << xy[0] << ' ' << xy[1];
	return 0;
}