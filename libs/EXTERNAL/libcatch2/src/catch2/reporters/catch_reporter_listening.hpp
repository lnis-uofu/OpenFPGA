
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE_1_0.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0
#ifndef CATCH_REPORTER_LISTENING_HPP_INCLUDED
#define CATCH_REPORTER_LISTENING_HPP_INCLUDED

#include <catch2/interfaces/catch_interfaces_reporter.hpp>

namespace Catch {

    class ListeningReporter final : public IStreamingReporter {
        using Reporters = std::vector<IStreamingReporterPtr>;
        Reporters m_listeners;
        IStreamingReporterPtr m_reporter = nullptr;

    public:
        ListeningReporter( IConfig const* config ):
            IStreamingReporter( config ) {
            // We will assume that listeners will always want all assertions
            m_preferences.shouldReportAllAssertions = true;
        }


        void addListener( IStreamingReporterPtr&& listener );
        void addReporter( IStreamingReporterPtr&& reporter );

    public: // IStreamingReporter

        void noMatchingTestCases( StringRef unmatchedSpec ) override;
        void fatalErrorEncountered( StringRef error ) override;
        void reportInvalidArguments( StringRef arg ) override;

        void benchmarkPreparing( StringRef name ) override;
        void benchmarkStarting( BenchmarkInfo const& benchmarkInfo ) override;
        void benchmarkEnded( BenchmarkStats<> const& benchmarkStats ) override;
        void benchmarkFailed( StringRef error ) override;

        void testRunStarting( TestRunInfo const& testRunInfo ) override;
        void testCaseStarting( TestCaseInfo const& testInfo ) override;
        void testCasePartialStarting(TestCaseInfo const& testInfo, uint64_t partNumber) override;
        void sectionStarting( SectionInfo const& sectionInfo ) override;
        void assertionStarting( AssertionInfo const& assertionInfo ) override;

        void assertionEnded( AssertionStats const& assertionStats ) override;
        void sectionEnded( SectionStats const& sectionStats ) override;
        void testCasePartialEnded(TestCaseStats const& testInfo, uint64_t partNumber) override;
        void testCaseEnded( TestCaseStats const& testCaseStats ) override;
        void testRunEnded( TestRunStats const& testRunStats ) override;

        void skipTest( TestCaseInfo const& testInfo ) override;

        void listReporters(std::vector<ReporterDescription> const& descriptions) override;
        void listTests(std::vector<TestCaseHandle> const& tests) override;
        void listTags(std::vector<TagInfo> const& tags) override;


    };

} // end namespace Catch

#endif // CATCH_REPORTER_LISTENING_HPP_INCLUDED
