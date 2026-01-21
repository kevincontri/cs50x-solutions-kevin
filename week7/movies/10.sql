SELECT name 
  FROM people 
  WHERE people.id IN 
  (SELECT person_id 
  FROM directors 
  JOIN movies 
  ON movies.id = directors.movie_id 
  JOIN ratings 
  ON movies.id = ratings.movie_id 
  WHERE ratings.rating >= 9.0);

