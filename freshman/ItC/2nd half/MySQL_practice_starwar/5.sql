SELECT P.Name FROM planets P
WHERE P.Name NOT IN (
    SELECT P.Name FROM planets P, timetable T
    WHERE P.Name = T.Planet_Name
);
