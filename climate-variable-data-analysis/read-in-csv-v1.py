import sys  # import for command-line parameters
import pandas as pd  # csv file reading

# handle command line parameters
raw_data = sys.argv[1]  # one year daily observation file
locations = sys.argv[3]  # mapping from location ID to state/county FIPS code
spatial = sys.argv[4]  # spatial resolution of output data: state, county
temporal = sys.argv[5]  # temporal resolution of output data: daily, weekly
output = sys.argv[6]  # output file

# handle reading in csv file
dfYearData = pd.read_csv(raw_data, header=None)
dfYearData.columns = ['location_id', 'date', 'variable', 'value', 'MFLAG', 'QFLAG', 'SFLAG', 'observation_time']
dfLocations = pd.read_csv(locations, header=None)
dfLocations.columns = ['location_id', 'fips']
df = pd.merge(dfLocations, dfYearData[['location_id', 'date', 'variable', 'value']])

print(dfYearData.head())
print(dfLocations.head())
print(df.head())


dfResult = df[df.variable == 'PRCP'][['fips', 'date', 'value']].groupby(['date','fips', 'variable']).mean().reset_index()
dfResult.to_csv('filename.csv', index=False)
