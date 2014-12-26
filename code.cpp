#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <map>
#include <vector>
#include <queue>
#include <deque>
#include <algorithm>
#include <string>
#include <iostream>
#include <map>
#include <deque>

using namespace std;

struct node{
	int idx, h;
};

class Solution {
	public:

		int height, width;
		int maximalRectangle(vector<vector<char> > &matrix) {
			int l[1000], r[1000], h[2][1000];
			int ans = 0;
			height = matrix.size();
			if (height > 0) {
				width = matrix[0].size();
			}
			else {
				return 0;
			}
			for (int i = 0; i < height; ++i) {
				memset(l, 0, sizeof(l));
				memset(r, 0, sizeof(r));
				for (int j = 0; j < width; ++j) {
					h[i%2][j] = 0;
				}

				for (int j = 0; j < width; ++j) {
					if (i == 0) {
						if (matrix[i][j] == '1') {
							h[i%2][j] = 1;
						}
						else {
							h[i%2][j] = 0;
						}
					}
					else {
						if (matrix[i][j] == '1') {
							h[i%2][j] = h[(i-1)%2][j] + 1;
						}
						else {
							h[i%2][j] = 0;
						}	
					}
				}
				deque<node> q;
				while(q.empty() == false) q.pop_back();
				for (int j = 0; j < width; ++j) {
					while(q.empty() == false && q.back().h >= h[i%2][j]) {
						q.pop_back();
					}
					if (q.empty() == false) {
						l[j] = j - q.back().idx - 1;
					}
					else {
						l[j] = j;
					}
					node now;
					now.h = h[i%2][j];
					now.idx = j;
					q.push_back(now);
				}
				while(q.empty() == false) q.pop_back();
				for (int j = width; j >= 0; --j) {
					while(q.empty() == false && q.back().h >= h[i%2][j]) {
						q.pop_back();
					}
					if (q.empty() == false) {
						r[j] = q.back().idx - j - 1;
					}
					else {
						r[j] = width - 1 - i;
					}
					ans = max(ans, h[i%2][j] * (l[j] + r[j] + 1));
					node now;
					now.h = h[i%2][j];
					now.idx = j;
					q.push_back(now);
				}

			}
			return ans;
		}
};

int main(){
	Solution *x = new Solution();
	vector<vector<char> > matrix;
	vector<char> vx;
	vx.push_back('0');
	vx.push_back('0');
	vx.push_back('0');
	matrix.push_back(vx);
	matrix.push_back(vx);
	matrix.push_back(vx);
	matrix.push_back(vx);
	cout << x->maximalRectangle(matrix) << endl;
	return 0;
}

