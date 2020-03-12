/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef TEST_TESTSUPPORT_PERF_TEST_H_
#define TEST_TESTSUPPORT_PERF_TEST_H_

#include <sstream>
#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "api/array_view.h"
#include "rtc_base/numerics/samples_stats_counter.h"

namespace webrtc {
namespace test {

enum class ImproveDirection {
  // Direction is undefined.
  kNone,
  // Smaller value is better.
  kSmallerIsBetter,
  // Bigger value is better.
  kBiggerIsBetter,
};

// Prints a performance test result.
//
// For example,
// PrintResult("ramp_up_time_", "turn_over_tcp",
//             "bwe_15s", 1234.2, "ms", false);
//
// will show up in the http://chromeperf.appspot.com under
//
// (test binary name) > (bot) > ramp_up_time_turn_over_tcp > bwe_15s.
//
// The |measurement| + |modifier| is what we're measuring. |user_story| is the
// scenario we're testing under.
//
// The binary this runs in must be hooked up as a perf test in the WebRTC
// recipes for this to actually be uploaded to chromeperf.appspot.com.
void PrintResult(const std::string& measurement,
                 const std::string& modifier,
                 const std::string& user_story,
                 const double value,
                 const std::string& units,
                 bool important,
                 ImproveDirection improve_direction = ImproveDirection::kNone);

// Like PrintResult(), but prints a (mean, standard deviation) result pair.
// The |<values>| should be two comma-separated numbers, the mean and
// standard deviation (or other error metric) of the measurement.
// DEPRECATED: soon unsupported.
void PrintResultMeanAndError(
    const std::string& measurement,
    const std::string& modifier,
    const std::string& user_story,
    const double mean,
    const double error,
    const std::string& units,
    bool important,
    ImproveDirection improve_direction = ImproveDirection::kNone);

// Like PrintResult(), but prints an entire list of results. The |values|
// will generally be a list of comma-separated numbers. A typical
// post-processing step might produce plots of their mean and standard
// deviation.
void PrintResultList(
    const std::string& measurement,
    const std::string& modifier,
    const std::string& user_story,
    rtc::ArrayView<const double> values,
    const std::string& units,
    bool important,
    ImproveDirection improve_direction = ImproveDirection::kNone);

// Like PrintResult(), but prints a (mean, standard deviation) from stats
// counter. Also add specified metric to the plotable metrics output.
void PrintResult(const std::string& measurement,
                 const std::string& modifier,
                 const std::string& user_story,
                 const SamplesStatsCounter& counter,
                 const std::string& units,
                 const bool important,
                 ImproveDirection improve_direction = ImproveDirection::kNone);

// If --write_histogram_proto_json=false, this returns all perf results to date
// in a JSON string formatted as described in dashboard/docs/data-format.md
// in https://github.com/catapult-project/catapult/blob/master/. If
// --write_histogram_proto_json=true, returns a string-encoded proto as
// described in tracing/tracing/proto/histogram.proto in
// https://github.com/catapult-project/catapult/blob/master/.
std::string GetPerfResults();

// Print into stdout plottable metrics for further post processing.
// |desired_graphs| - list of metrics, that should be plotted. If empty - all
// available metrics will be plotted. If some of |desired_graphs| are missing
// they will be skipped.
void PrintPlottableResults(const std::vector<std::string>& desired_graphs);

// Call GetPerfResults() and write its output to a file. Returns false if we
// failed to write to the file.
bool WritePerfResults(const std::string& output_path);

// By default, perf results are printed to stdout. Set the FILE* to where they
// should be printing instead.
void SetPerfResultsOutput(FILE* output);

// Only for use by tests.
void ClearPerfResults();

}  // namespace test
}  // namespace webrtc

// Only for use by tests.
ABSL_DECLARE_FLAG(bool, write_histogram_proto_json);

#endif  // TEST_TESTSUPPORT_PERF_TEST_H_
