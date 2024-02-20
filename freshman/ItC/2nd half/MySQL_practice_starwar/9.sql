SELECT K.Movie, K.Race, K.Character_Name
FROM TimeTable T, (
    SELECT DISTINCT T.Movie, T.Character_Name, C.Race, COUNT(T.Character_Name) AS num
    FROM TimeTable T, Characters C
    WHERE C.Name = T.Character_Name
    GROUP BY T.Movie, T.Character_Name, C.Race
) K, (
    SELECT DISTINCT Kt.Movie, Kt.Race, MAX(Kt.num) AS maxnum
    FROM (
        SELECT DISTINCT T.Movie, T.Character_Name, C.Race, COUNT(T.Character_Name) AS num
        FROM TimeTable T, Characters C
        WHERE C.Name = T.Character_Name
        GROUP BY T.Movie, T.Character_Name, C.Race
    ) Kt
    GROUP BY Kt.Movie, Kt.Race
) KM
WHERE K.num = KM.maxnum
    AND K.Movie = KM.Movie
    AND K.Race = KM.Race
GROUP BY K.Race, K.Movie, K.Character_Name
ORDER BY K.Race, K.Movie ASC;
