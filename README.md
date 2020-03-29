# covidAnalysis

Tracks growth rates of coronavirus COVID-19 cases, based on data from
[https://github.com/CSSEGISandData/COVID-19](https://github.com/CSSEGISandData/COVID-19)

The growth rate is the number of new cases on day N divided by the number of new cases
on day N-1.

## Setup
* clone the above repository next to this one
  * `pushd ..; git clone https://github.com/CSSEGISandData/COVID-19.git; popd`
* prerequisites
  * this uses `python` to parse `csv` files, and `CERN ROOT` for the plotting

## Analysis
* update data: `update.sh`
* plot data: `analyse.sh __country__`
  * example: `analyse.sh Italy`
  * example: `analyse.sh US`
  * an optional second argument may be passed: this is the day number 
    used to specify the starting day of the growth factor curve fit
    * example: `analysis.sh US 57`

