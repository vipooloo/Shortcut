
#include "JobScMgr.h"
#include <iostream>

int main(int argc, char* argv[])
{
    JobScMgr::Add(123456789, JobScItem());

    JobScPageQuery query;
    JobScPageResult page_result;
    std::vector<JobScItem> items;
    query.SetType(JobScType::All);
    JobScMgr::GetListByTypePage(query, page_result, items);
    return 0;
}