#include "Mutation.h"
#include "loading.cpp"
#include <random>

#ifndef BASEMUT_H
#define BASEMUT_H

struct CalMutation : Mutation {
    std::default_random_engine generator;
    std::uniform_int_distribution<size_t> indexDist;
    size_t safetymin = 0;

    void mutate(std::string &contents, std::map<std::string, std::any> options) override;
};

struct ChunkRandomize : CalMutation{
    void mutate(std::string &contents, std::map<std::string, std::any> options) override;
};

struct MoveChunks : CalMutation {
    void mutate(std::string &contents, std::map<std::string, std::any> options) override;
};

struct RepeatChunks : CalMutation {
    void mutate(std::string &contents, std::map<std::string, std::any> options) override;
};

struct RemoveChunks : CalMutation {
    void mutate(std::string &contents, std::map<std::string, std::any> options) override;
};

struct ReverseChunks : CalMutation {
    void mutate(std::string &contents, std::map<std::string, std::any> options) override;
};

struct NullChunks : CalMutation {
    void mutate(std::string &contents, std::map<std::string, std::any> options) override;
};

struct SwapChunks : CalMutation {
    void mutate(std::string &contents, std::map<std::string, std::any> options) override;
};

struct IncrementBytes : CalMutation {
    void mutate(std::string &contents, std::map<std::string, std::any> options) override;
};

struct RainbowSpread : CalMutation {
    void mutate(std::string &contents, std::map<std::string, std::any> options) override;
};

struct EchoWave : CalMutation {
    void mutate(std::string &contents, std::map<std::string, std::any> options) override;
};

struct AverageChunks : CalMutation {
    void mutate(std::string &contents, std::map<std::string, std::any> options) override;
};

#endif