#pragma once

/** \file
 * This file defines concrete implementations of the TimingAnalyzer interfaces.
 *
 * In particular these concrete analyzers are 'full' (i.e. non-incremental) timing analyzers,
 * ever call to update_timing_impl() fully re-analyze the timing graph.
 */

#include "FullSetupTimingAnalyzer.hpp"
#include "FullHoldTimingAnalyzer.hpp"
#include "FullSetupHoldTimingAnalyzer.hpp"
