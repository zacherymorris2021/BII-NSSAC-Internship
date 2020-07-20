# This file aggregates weather data and graphs flu activity level and a
# weather variable for any state between 2008 to 2019
# Created by Zachery Morris, 2019

import pandas as pd
import matplotlib.pyplot as plt
from datetime import timedelta


df_cdc_state_level = pd.read_csv('cdc-state-level-20190610.csv', sep='|', parse_dates=['weekend'])
df_cdc_state_level = df_cdc_state_level[df_cdc_state_level.activity_level > 0]

df_cdc_state_level['date_by_wednesday'] = df_cdc_state_level['weekend'].apply(lambda x: x-timedelta(days=3)) # moves from each week by weekend to every wednesday
df_fips = pd.read_csv('state_fips.csv', dtype={'state': str}) # state == state's fips code
df_fips_included = pd.merge(df_cdc_state_level, df_fips, left_on='statename', right_on='state_name')

df_state_weekly_PRCP = pd.read_csv('state-weekly-TMIN.csv', sep=',', dtype={'fips': str}, parse_dates=['date']) # change file to aggregate precipitation, max temp, ect
print(df_state_weekly_PRCP.head(10))
df_weekly_PRCP_included = pd.merge(df_fips_included, df_state_weekly_PRCP, left_on='date_by_wednesday', right_on='date')

df_weekly_PRCP_included[['date', 'activity_level', 'fips', 'value', 'statename']].to_csv('filename.csv', index=False)
df_weekly_PRCP_included2 = df_weekly_PRCP_included[['date', 'activity_level', 'fips', 'value', 'statename', 'season']]
df_weekly_PRCP_included2['value'] = df_weekly_PRCP_included2['value'] / 20
df_weekly_PRCP_included2.sort_values('date', inplace=True)

# graph flu activity with min temperature for Virginia during 2018-2019 -- change file above to graph precipitation and max temp
df_state = df_weekly_PRCP_included2[(df_weekly_PRCP_included2['statename'] == 'Virginia')]
df_new = df_state[(df_state['season'] == '2018-19')]
plt.plot(df_new['date'], df_new['value'], c="DARKORANGE", label='TMIN, in tenths of degrees Celsius (Scaled)')
plt.plot(df_new['date'], df_new['activity_level'], c="DARKBLUE", label='Activity Level')
plt.tick_params(labelsize=10)
plt.legend()
plt.xlabel("Dates between 2018-2019", fontsize=11)
plt.ylabel("Scaled Value", fontsize=11)
plt.xticks(['2018-10-03','2019-01-23','2019-05-18'])
plt.grid()
plt.title("Influenza Activity Level and Minimum Temperature vs Time\n in Virginia")
plt.savefig('Virginia_minTemp.png')


# Uncomment to create a graph for each state with only activity level -- preliminary code

# arr = df_fips_included['statename'].unique()
# for state in arr:
#     df_state = df_fips_included[(df_fips_included['statename'] == state)]
#     df_state.plot(kind='line', x='date_by_wednesday', y='activity_level', color='royalblue')
#     plt.suptitle('Influenza Activity Level, Precipitation, Temperature vs Time\nfor ' + state)
#     plt.ylabel('Scaled Value')
#     plt.xlabel('Date')
#     plt.grid('on')
#     plt.legend()
#     plt.savefig(state + 'Plot.png', bbox_inches='tight')


