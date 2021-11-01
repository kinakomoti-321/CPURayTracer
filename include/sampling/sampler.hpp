#pragma once

//乱数生成器
class Sampler {
 public:
  virtual float sample() const = 0;
  virtual void reset(uint64_t inseed) const = 0;
};