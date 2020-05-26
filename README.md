<h1>chess.c</h1>
<h2>Background</h2>
<p>
  For completeness, a description of chess is provided here: https://en.wikipedia.org/wiki/Chess
</p>

<br>
<h2>Code</h2>
<p>
  This is an implimentation of chess which runs on the terminal. Users take turns to enter letter-number coordinate pairs to indicate   which piece they would like to move. and where they would like to move it. The program validates these pairs of coordinates according to the chess rulebook specifications.
  
  The main objective of this project was to develop a good understanding of the pointer system, to practise DRY code. I focussed on keeping functions short and thus easily testable using 'assert'.
  
  The code is all inside of the chess.c plaintext.
</p>
<br>
<h2>Reflection</h2>
<p>
  The game does have some bugs where invalid moves are flagged up as vali an vice-versa. The time-cost of hunting own each bug in the validation methods didn't seem worth the benefit of a more functional chess clone.
  
  Despite this I got significant benefit from using the 'assert' library. Figuring out a good specification for each function and writing a test function using assertions BEFORE writing any code for the function itself proved an excellent way to spot mistakes and solidify a plan as I went.
  
  With retrospect, it would have been prudent to split some segments of code into separate modules for simplicity and (obviously) modularity. More concretely, testing should have been separated completely, along with validation and the main runner.
</p>
