# Forked

Forked is an esoteric stack-based two-dimensional language with multiple IPs,
based on [Triangular](//git.io/triangular) and written around the [fork command](#Fork).
Instructions are single-character commands.

**Forked is an unimplemented WIP language. This repository just contains the spec.**

 - **If you would like to contribute to this language, please fork (ba dum crash) and PR.**
 - **If you have an opinion on a "TODO" item, or something is unclear, please open an issue.**
 - **Do not implement this language. It changes very frequently.**

# Commands

General commands:

    | no-op if not next to a fork
    - no-op if not next to a fork
    & exit

Directionals:

    v redirect down
    ^ redirect up
    > redirect right
    < redirect left
    \ bounce IP
        if hit from left, bounce down and vice versa
        if hit from right, bounce up and vice versa
    / bounce IP
        if hit from left, bounce up and vice versa
        if hit from right, bounce down and vice versa

I/O:

    $ read input as integer
    ~ read input as character (-1 if EOF)
    % print ToS as integer
    @ print ToS as character
    ? print ToS as integer and pop
    ! print ToS as character and pop

Stack:

    i increment ToS
    d decrement ToS
    + postfix ADD       pop values used, push result
    ' postfix SUBTRACT  pop values used, push result
    _ postfix DIVIDE    pop values used, push result
    * postfix MULTIPLY  pop values used, push result
    = postfix ISEQUAL   pop values used, push result
    m postfix MODULO    pop values used, push result
    l postfix LESSTHAN  pop values used, push result
    g postfix MORETHAN  pop values used, push result    (heh mlg)
    0 push 0 to stack. 1 pushes 1, 2 pushes 2, etc.
    A push 10 to stack. B pushes 11, ... F pushes 15.
    d duplicate ToS
    p pop ToS
    . pop the stack index stored in the ToS
    , pop the stack index stored in the ToS, then the ToS
    

Loops:

    { if entered (with `v^><|-`) from top or left, create jump point
        otherwise, unconditionally jump back to most recently created jump point
    } if entered (with `v^><|-`) from top or left, unconditionally jump back to the most recently created jump point
        otherwise, create jump point

Memory:

    P pop ToS into register
    S stash ToS in register
    U pull register to stack
    O pop register

IP:

    ; IP fork (see IP section)
    [ switch to next IP
    ] switch to previously created IP
    ( create new IP here, switch to it
    ) discard current IP, switch to previous (exits with nonzero return value if IP is 0)
    I jump to IP specified in the top stack value
    ` bomb: destroy all instruction pointers except 0
            if top of stack is truthy, set IP 0 to current point and run
            otherwise, run IP 0 without changing its location

# Fork

    : fork the program
    ; IP-fork the program
    # random-fork the program

The fork is the only conditional.
It may be entered from any direction, and changes the direction of the IP according to the value on the top of the stack.
**Jumps cannot be used to enter or exit forks.**

    -:  enters the fork from the East

     :- enters the fork from the West

     |
     :  enters the fork from the North

     :  enters the fork from the South
     |

If the value on the top of the stack is truthy, the instruction pointer turns right; otherwise it turns left.

|Fork entered from...|New IP direction if truthy|New IP direction if falsy|
|-|-|-|
|West|North|South|
|East|South|North|
|North|East|West|
|South|West|East|

Similar to entry points, a character (`|` for North/South or `-` for East/West) is required to be at both possible
exit points. If `|` or `-` is not present at both exit points (right and left of the entry point), or the incorrect
character is used for an exit, the program will terminate with a nonzero return value. For example:

      v
      |
      >--:

That will exit with error, as will this (even though the fork will doubtlessly redirect the IP North):

      v  |
      |  |
      >--:

This will also:

      v
      |  -----
      >--:
         >----

The random fork, `#`, acts exactly like the fork command `:`, except picks randomly between the two available directions.

# IPs

Forked has an infinite number of instruction pointers, each named by their index.
IP 0 is created at the first (upper left) character in the source code, travelling right.

If an IP hits the end of a playing field (a newline,) it will wrap around and continue going the same direction.
For example:

    blah blah blah blah blah blah blah blah
    foo bar foo bar foo bar foo bar foo bar

will be infinitely read as "blah blah blah blah blah blah blah blah"
(assuming `b`, `l`, `a`, and `h` do not change the direction).

**TODO:** Should IPs wrap, or should it be discarded and go to the previous IP?

The terminate command `&` acts exactly like `]` when reached with an IP other than 0.

There is also the IP fork, `;`. It acts the same as `:` except when hit, it creates a new IP.
The IPs are created at the two possible travel points of the IP after a fork. For example:

      v   |
      1   |  <- new IP created here, does not run
      >---;
          |  <- old IP continues running here
          |
          >-&  <- program terminates, IP is never used

Another example:

          >---%-&  <- print top of stack, delete IP 1 and jump to IP 0
      v   |
      1   |  <- new IP (1) created here
      >---;
          |  <- old IP (0) continues here
          |
          >---I-&  <- top of stack = 1, so IP 0 stops and IP 1 runs

Bomb example:

          >--%`&  <- print '1', destroy IP 1, set IP 0 here (top of stack = 1) and exit
      v   |
      1   |  <- new IP (1) created here
      >---;
          |  <- old IP (0) continues here
          |
          >---I  <- top of stack stack = 1, so IP 0 stops and IP 1 runs

# Fork illustrations

This doesn't do anything.

        v       direct down
        $       read input
        |       enter fork from top
    &---:--&    split, go left if truthy, right if falsy

Neither does this.

      v
      $    >-----&
      |    |
      >----:        enter fork from left, fork up if falsy, down if truthy
           |
    &------<

This runs infinitely in a very messy way but illustrates the language quite well.

      V
      |
      |
      |
    /-:-\
    | | |
    | | |
    | | |

(The code is read as `v|||:-`, then `/|||/\|||\-:-` infinitely.)

# Examples (golfy)

Truth machine:

{% raw %}

        v
        $
        |
    {%}-:-%&

{% endraw %}

Cat program:

    >-v
    @ ~
    | | 
    \-:-&

Reverse input:

    >-v
    | ~
    | |
    \-:-v
    &{!}<
