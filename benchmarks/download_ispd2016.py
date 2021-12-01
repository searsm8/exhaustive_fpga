# Script to automatically download benchmarks for ISPD 2016
# The contest was for Routability Driven FPGA placement,
# so these benchmarks are small, medium, and large FPGA designs

import os, tarfile
import urllib.request

ISPD2016_EXAMPLE_BENCHMARKS_URL = "https://www.ispd.cc/contests/16/benchmarks/"
benchmark_example_names_2016 = [f"FPGA-example{i}.tar.gz" for i in range (1, 5)]
ISPD2016_BENCHMARKS_URL = "https://www.ispd.cc/contests/17/downloads/2016/"
benchmark_names_2016 = [f"FPGA{i:02d}.tar.gz" for i in range (1, 13)]

def fetch_benchmarks(URL=ISPD2016_BENCHMARKS_URL, path="ISPD_2016", benchmark_list=benchmark_names_2016):
    os.makedirs(path, exist_ok=True)
    for benchmark in benchmark_list:
        tar_gz_path = os.path.join(path, benchmark)
        benchmark_URL = URL + benchmark
        print("Downloading from", benchmark_URL)
        urllib.request.urlretrieve(benchmark_URL, tar_gz_path)
        benchmark_tgz = tarfile.open(tar_gz_path)
        print("Unzipping", tar_gz_path)
        benchmark_tgz.extractall(path=path)
        benchmark_tgz.close()


if __name__ == "__main__":
    # for ISPD 2016
    print(benchmark_example_names_2016)
    fetch_benchmarks(URL=ISPD2016_EXAMPLE_BENCHMARKS_URL, path="ISPD_2016", benchmark_list=benchmark_example_names_2016)

    print(benchmark_names_2016)
    fetch_benchmarks(URL=ISPD2016_BENCHMARKS_URL, path="ISPD_2016", benchmark_list=benchmark_names_2016)

    # for ISPD 2022
    #ISPD2022_BENCHMARKS_URL = "https://drive.google.com/file/d/1comBzVt6TXEpvEoasZVm0C2NApKhBl3z/view?usp=sharing"
    #fetch_benchmarks(ISPD2022_BENCHMARKS_URL, path="ISPD_2022", benchmark_list=[""])