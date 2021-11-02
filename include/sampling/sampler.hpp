#pragma once

//乱数生成器
class Sampler {
 public:
  virtual float sample() = 0;
  virtual void reset(uint64_t inseed) = 0;
};