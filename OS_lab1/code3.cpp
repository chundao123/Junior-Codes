class Solution {
public:
    vector<int> maxInWindows(const vector<int>& num, unsigned int size) {
        if(size=0)
            return vector<int>{};
        deque<int> Q;
        int n=num.size();
        vector<int> res;
        for(int ii=0; ii<n; ++ii)
        {
            while(!Q.empty()&&num[Q.back()]<=num[ii])
                Q.pop_back();
            Q.push_back(ii);
            if(Q.front()+size==ii)
                Q.pop_front();
            if(ii>size-2)
                res.push_back(num[Q.front()]);
        }
        return res;
    }
};
