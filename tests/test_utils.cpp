#include <string>
#include <catch2/catch.hpp>
#include <utils.hpp>

TEST_CASE( "2: String representation of price time series (empty)", "[multi-file:2]" ) 
{
    SECTION( "time series with data" )
    {
        auto const &some_pT =
            bt::price_t{14.5, 10.4, 11.4, 16.4, 10.5, 20.7};
        auto const &str_rep = bt::ts_to_str(some_pT);
        
        auto const &ref_rep = 
            "[14.5, 10.4, 11.4, 16.4, 10.5, 20.7]";
            
        REQUIRE( str_rep == ref_rep );
    }

}