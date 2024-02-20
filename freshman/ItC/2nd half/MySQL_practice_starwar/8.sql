SELECT DISTINCT K.Movie, K.Planet_Name
FROM Timetable T, Characters C, Planets P, 
    (
    SELECT T.Movie, T.Planet_Name, T.Character_Name, (T.Time_of_Departure - T.Time_of_Arrival) AS num 
    FROM TimeTable T 
    ) K, 
    (
        SELECT Kt.Movie AS maxMovie, MAX(Kt.num) AS maxnum
        FROM (
            SELECT T.Movie, T.Planet_Name, T.Character_Name, (T.Time_of_Departure - T.Time_of_Arrival) AS num 
            FROM TimeTable T 
        ) Kt
        WHERE Kt.Character_Name = 'Luke Skywalker'
        GROUP BY Kt.Movie
    ) KM
WHERE K.Character_Name = 'Luke Skywalker'
    AND (K.Planet_Name = P.Name AND C.Name = 'Luke Skywalker' AND C.Affiliation != P.Affiliation)
    AND K.num = KM.maxnum
    AND K.Movie = KM.maxMovie
ORDER BY K.Movie;