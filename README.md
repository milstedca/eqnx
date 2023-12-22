This project is a clone of the official GNU groff project. See the other README files for the
copyrights and other legaleze.

I have cloned this project in order to make updates to the eqn command. That's where you will find
my changes. When in MathML mode the eqn keywords commands approx and xi failed. Once I dug into the
code to find the bugs, I realized that it wouldn't be hard to add a boatload of symbols to the
eqn vocabulary. It would be nice to make eqn nearly as powerful as TeX. TeX is hard to type.

So my plan with this project is to  extend eqn to support the symbols that TeX supports to the
greatest degree feasible without making really hard/dangerous mods. 

As of late December 2023, I have added in a large number of symbols, using the same names
as TeX sans annoying backslashes. To use the additions, build using the eqnx branch. (I am
trying to keep the master branch close to GNU eqn, with just my bug fixes added.) Also,
you can style Latin into different fonts using prefixes: ds for double stroke, frak for Fraktur,
bfrak for bold Fraktur, ss for sans serif, bss for bold sans serif, scr for script, and bscr
for bold script.

This violates the original convention of having a keyword for font changes, but its reasonably
easy to type, and very easy to fit into the existing framework. I rely on Unicode code points
vs. swapping fonts in order to get the font changes. This was not an option when troff was invented!

Also, to improve performance, the mapping of macros to MathML entities has been updated. It was
a statically created array, with a linear search. This got slow when increased the number of
built in symbols. So I replaced the array with a modern std::map. 

To see the extensions in action go here: https://conntects.net/members/thatsDoctorMilsted/postPermalinks/118
