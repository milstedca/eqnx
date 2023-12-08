This project is a clone of the official GNU groff project. See the other README files for the
copyrights and other legalize.

I have cloned this project in order to make updates to the eqn command. That's where you will find
my changes. When in MathML mode the eqn keywords commands approx and xi failed. Once I dug into the
code to find the bugs, I realized that it wouldn't be hard to ad a boatload of symbols to the
eqn vocabulary. It would be nice to make eqn nearly as powerful as TeX. TeX is hard to type.

So my plan with this project is to  extend eqn to support the symbols that TeX supports to the
greatest degree feasible without making really hard/dangerous mods. 

Then, if that goes smoothly enough, I might add some more complicated features such as
subscripts and superscripts on the left side of symbols. Might even support cube roots, but
don't hold your breath.
