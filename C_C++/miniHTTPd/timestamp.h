#pragma once

#include <inttypes.h>
#include <stdio.h>

void timestamp_toString(char *buf, size_t size, int64_t microSecondsSinceEpoch);

void timestamp_toFormattedString(char *buf, size_t size, int64_t microSecondsSinceEpoch, int showMicroseconds);

// return microseconds
int64_t timestamp_now();

// return (high-low) in seconds, high and low are microseconds
double timestamp_difference(int64_t high, int64_t low);
