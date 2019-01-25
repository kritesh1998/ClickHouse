#pragma once
#include <string>
#include <vector>
#include <map>
#include <Interpreters/Settings.h>
#include <Poco/Util/XMLConfiguration.h>
#include <Poco/AutoPtr.h>

#include "StopConditionsSet.h"
#include "TestStopConditions.h"
#include "TestStats.h"

namespace DB
{
enum class ExecutionType
{
    Loop,
    Once
};

using XMLConfiguration = Poco::Util::XMLConfiguration;
using XMLConfigurationPtr = Poco::AutoPtr<XMLConfiguration>;
using StringToVector = std::map<String, std::vector<String>>;

class PerformanceTestInfo
{
public:
    PerformanceTestInfo(XMLConfigurationPtr config, const std::string & profiles_file_);

    std::string test_name;
    std::string main_metric;

    std::vector<std::string> queries;
    std::vector<std::string> metrics;

    Settings settings;
    ExecutionType exec_type;
    StringToVector substitutions;
    size_t times_to_run;
    std::string profiles_file;
    std::vector<TestStopConditions> stop_conditions_by_run;

private:
    void applySettings(XMLConfigurationPtr config);
    void extractQueries(XMLConfigurationPtr config);
    void processSubstitutions(XMLConfigurationPtr config);
    void getExecutionType(XMLConfigurationPtr config);
    void getStopConditions(XMLConfigurationPtr config);
    void getMetrics(XMLConfigurationPtr config);
};

}
