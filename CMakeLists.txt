cmake_minimum_required(VERSION 3.22)
project(Tweet_Generator)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Tweet_Generator
        justdoit_tweets.txt
        linked_list.c
        linked_list.h
        markov_chain.h
        tweets_generator.c
        markov_chain.c)
