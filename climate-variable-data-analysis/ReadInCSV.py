import sys  # import for command-line parameters
import pandas as pd  # csv file reading

# handle command line parameters
raw_data = sys.argv[1]  # one year daily observation file
variable = sys.argv[2] # climatic variable: PRCP, TMAX, TMIN, TOBS
locations = sys.argv[3]  # mapping from location ID to state/county FIPS code, file will
                         # either be state fips or county fips
temporal = sys.argv[4]  # temporal resolution of output data: daily, weekly
output = sys.argv[5]  # output file

# handle reading in csv file
dfYearData = pd.read_csv(raw_data, header=None)
dfYearData.columns = ['location_id', 'date', 'variable', 'value', 'MFLAG', 'QFLAG', 'SFLAG', 'observation_time']
dfLocations = pd.read_csv(locations, header=None)
dfLocations.columns = ['location_id', 'fips']
print(dfYearData.head())
print(dfLocations.head())

df = pd.merge(dfYearData[['location_id', 'date', 'variable', 'value']], dfLocations)

print("-----")
print(df.head())

# handles daily
# if climatic variable=PRCP & temporal=daily
if (variable == "PRCP") & (temporal == "daily"):
    dfResult = df[df.variable == 'PRCP'][['fips', 'date', 'variable', 'value']].groupby(['date', 'fips', 'variable']).mean().reset_index()
    dfResult.to_csv('filename.csv', index=False)

# if climatic variable=TMAX & temporal=daily
if (variable == "TMAX") & (temporal == "daily"):
    dfResult = df[df.variable == 'TMAX'][['fips', 'date', 'variable', 'value']].groupby(['date', 'fips', 'variable']).mean().reset_index()
    dfResult.to_csv('filename.csv', index=False)

# if climatic variable=TMIN & temporal=daily
if (variable == "TMIN") & (temporal == "daily"):
    dfResult = df[df.variable == 'TMIN'][['fips', 'date', 'variable', 'value']].groupby(['date', 'fips', 'variable']).mean().reset_index()
    dfResult.to_csv('filename.csv', index=False)

# if climatic variable=TOBS & temporal=daily
if (variable == "TOBS") & (temporal == "daily"):
    dfResult = df[df.variable == 'TOBS'][['fips', 'date', 'variable', 'value']].groupby(['date', 'fips', 'variable']).mean().reset_index()
    dfResult.to_csv('filename.csv', index=False)

# handles weekly



