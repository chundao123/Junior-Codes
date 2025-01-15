#include "../head.h"

VALUE_TYPE binaryQueryInSequential(const string& strQuery, ifstream& fin, int iDataSize)
{
	VALUE_TYPE value=0;
	string key;
	int low = 0;
	int high = iDataSize - 1;
	int mid;

	while(low <= high)
	{
		mid = low + (high - low) / 2;
		fin.seekg(mid * iLineSize, ios::beg);
		fin >> key >> value;

		if(key == strQuery)
			return value;
		else if(key < strQuery)
			low = mid;
		else if(key > strQuery)
			high = mid;
	}

	return -1;
}

class Index
{
public:
	struct Entry{
		string key;
		int offset;
	};

	vector<vector<Entry>> Layers;
	int group_size;

	void buildIndex(string filename, int _iDataSize, int _iLayers)
	{
		ifstream fin(filename);
		group_size = pow(_iDataSize, 1.0 / (_iLayers + 1));

		vector<Entry> layer_0;
		for(int i = 0; i < _iDataSize; i += group_size)
		{
			fin.seekg(i * iLineSize, ios::beg);
			string key;
			VALUE_TYPE value;
			fin >> key >> value;
			layer_0.push_back({key, i});
		}
		Layers.push_back(layer_0);

		for(int layer = 0; layer < _iLayers - 1; layer++)
		{
			vector<Entry> next_layer;
			for(int i = 0; i < Layers[layer].size(); i += group_size)
			{
				next_layer.push_back(Layers[layer][i]);
			}
			Layers.push_back(next_layer);
		}
	}

	VALUE_TYPE search(string strQuery, ifstream & fin)
	{
		VALUE_TYPE value = 0;
		string key;
		int layer = Layers.size() - 1;
		int start = 0;

		while(layer >= 0)
		{
			key = Layers[layer][start].key;
			if(key <= strQuery)
			{
				start ++;
			}
			if(key > strQuery || start >= Layers[layer].size())
			{
				layer --;
				start --;
				if(layer >= 0)
					start *= group_size;
			}
		}

		for(int i = Layers[0][start].offset; i < Layers[0][start].offset + group_size; i++)
		{
			fin.seekg(i * iLineSize, ios::beg);
			fin >> key >> value;
			if(key == strQuery)
				return value;
		}
		
		return -1;
	}

private:
//you can add other members or functions
};



