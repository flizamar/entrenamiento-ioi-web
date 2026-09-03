#!/bin/bash

problem="nile"
grader_name="grader"

g++ -std=gnu++17 -Wall -O2 -pipe -g -o "${problem}" "${grader_name}.cpp" "${problem}_sofi.cpp"