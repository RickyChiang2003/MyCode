SELECT K.Character_Name, K.num
FROM Characters C,(
    SELECT T.Planet_Name, T.Character_Name, COUNT(T.Planet_Name) AS num 
    FROM TimeTable T 
    GROUP BY T.Planet_Name, T.Character_Name
    ) K
WHERE C.Homeworld = K.Planet_Name
    AND C.Name = K.Character_Name;
