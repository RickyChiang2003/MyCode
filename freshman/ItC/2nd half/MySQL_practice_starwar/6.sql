SELECT DISTINCT K.Movie, K.Character_Name
FROM Timetable T, 
    (SELECT T.Movie, T.Character_Name, COUNT(T.Planet_Name) AS num 
    FROM TimeTable T 
    GROUP BY T.Movie, T.Character_Name
    ) K
WHERE K.num IN (
    SELECT MAX(Kt.num) 
    FROM (
        SELECT T.Movie, T.Character_Name, COUNT(T.Planet_Name) AS num
        FROM TimeTable T 
        GROUP BY T.Movie, T.Character_Name
        ) Kt, Timetable T
    WHERE Kt.Movie = T.Movie
    GROUP BY T.Movie
    )
ORDER BY K.Movie ASC;
