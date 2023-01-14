# print usage function
function usage {
    echo "Usage: ./benchmark.sh <data_dir> <algo> (<time_limit>)"
    echo "Where algo can be one of these: --mip, --greedy, --bruteforce, --simulated_annealing"
    echo "<time_limit> is the time limit in seconds (default 600)."
    exit 1
}

# if the number of arguments is less than 2 or greater than 3, print the usage
if [ $# -lt 2 ] || [ $# -gt 3 ]; then
    usage
fi

# if the first argument is not a directory, print the usage
if [ ! -d $1 ]; then
    usage
fi

# if the second argument is not one of the algorithms, print the usage
if [ $2 != "--mip" ] && [ $2 != "--greedy" ] && [ $2 != "--bruteforce" ] && [ $2 != "--simulated_annealing" ]; then
    usage
fi

# if the third argument is not a number, print the usage
if [ $# -eq 3 ] && ! [[ $3 =~ ^[0-9]+$ ]]; then
    usage
fi

# if the user provides a time limit, use it
if [ $# -eq 3 ]; then
    time_limit=$3
else
    time_limit=600
fi

algo_dir="${2#*--}"
mkdir -p log  # create the output directory if it does not already exist
cd log && mkdir -p $algo_dir
echo `date` > $algo_dir/date.txt
cd ..

echo "Experimental Campaign:"
echo "Data directory: $1"
echo "Output directory: log/$algo_dir"
echo "Algorithm: $2"
echo "Time limit: $time_limit"
echo

# Compile the code to have the latest versions of the executables
mkdir -p build
cd build
cmake ..
echo
make 'unit_test.out'

# Run the unit tests and check that they pass, otherwise exit
./unit_test.out

if [ $? -ne 0 ]; then
    echo "Unit tests failed, exiting."
    exit 1
fi
echo

# Run the benchmark
echo "Unit tests passed, running the benchmark now..."
echo
# if the algorithm is mip, run the mip solver for each instance
if [ $2 == "--mip" ]; then
    make 'mip.out'
    echo
    # for all subdirectories in the data directory
    for dir in $1/*; do
        # create a variable instance that contains the name of the instance (split by '/')
        instance=${dir##*/}
        echo "Solving $instance"
        ./mip.out $dir $time_limit > ../log/$algo_dir/log_$instance.txt
    done
else
    make 'test.out'
    echo
    # otherwise, run the algorithm for each instance
    for dir in $1/*; do
        instance=${dir##*/}
        echo "Solving $instance"
        ./test.out $dir $2 $time_limit > ../log/$algo_dir/log_$instance.txt
    done
fi
echo
grep "Result:" ../log/$algo_dir/*.txt > ../log/$algo_dir/results.csv
echo "Benchmark finished, results are in log/$algo_dir/results.csv"
exit 0