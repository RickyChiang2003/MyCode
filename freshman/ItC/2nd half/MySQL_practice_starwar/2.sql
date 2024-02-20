SELECT DISTINCT T.Character_Name FROM timetable T
WHERE T.Character_Name NOT IN (
    SELECT DISTINCT T.Character_Name FROM timetable T, planets P
    WHERE T.Planet_Name = P.Name AND P.Affiliation = 'empire'
);

