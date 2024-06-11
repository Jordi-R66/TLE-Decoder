@echo off
cls

set "SETS=active stations last-30-days visual analyst cosmos-1408-debris fengyun-1c-debris iridium-33-debris cosmos-2251-debris weather noaa goes resource sarsat dmc tdrss argos planet spire geo gpz gpz-plus intelsat ses starlink iridium iridium-NEXT oneweb orbcomm globalstar swarm amateur x-comm other-comm satnogs gorizont raduga molniya gnss gps-ops glo-ops galileo beidou sbas nnss musson science geodetic engineering education military radar cubesat other"

echo Downloading every TLE

for %%f in (%SETS%) do (
    curl -x "172.16.0.253:3128" "https://celestrak.org/NORAD/elements/gp.php?GROUP=%%f&FORMAT=tle" > "TLEs/%%f.tle"
)

echo All files have been downloaded
