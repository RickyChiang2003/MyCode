SELECT K.Planet_Name
FROM (
    SELECT T.Planet_Name, COUNT(DISTINCT T.Character_Name) AS num
    FROM Timetable T
    GROUP BY T.Planet_Name
) K
WHERE K.num > 3