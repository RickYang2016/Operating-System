#include <iostream>
#include <fstream>
#include <iterator>
#include <unistd.h>
#include <getopt.h>

using namespace std;
 
int main(int argc, char **argv)
{
	string type;
	string in_file;
	string out_file;
	string help;
	string sep("\t");
	int col = -1;
	double sum = 0.0; 
 
	if (argc > 1)
	{
		type = string(argv[1]);
	}

	int ch;
	opterr = 0; 

	while ((ch = getopt(argc, argv, "i:oh")) != -1)
	{
		switch (ch)
		{
			case 'i':
			{
				in_file = string(optarg);
				ifstream input( in_file );
				if( input )
				{
				    for( istream_iterator<double> itor(input); itor != istream_iterator<double>(); ++itor )
				    {
				        sum += *itor;
				    }
				    cout << sum << endl;
				}
				break;
			}
			case 'o':
			{
				//out_file = string(optarg); 
				ofstream output("output_part1.txt");
				output << sum << endl;
				output.close();
				break;
			}
			case 'h':
			{
				//help = string(optarg);
				cout << "Use the flag -i to add the input file. "
				"If you want to save the result, use the flag -o "
				"and the result will automatically save in the file nameing 'output_part1'." << endl;
				break;
			}
		}
	}
	return 0;
}
