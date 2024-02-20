SELECT DISTINCT P.Name FROM Planets P, Characters C, Timetable T
WHERE P.Name = T.Planet_Name AND
    C.NAME = T.Character_Name AND
    C.Affiliation = 'rebels';

