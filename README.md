# Apter Trees in C++

Apter Trees are a representation of trees using just two vectors: `[nodevalues,
parentindices]`.

This repo contains a tree-like data type implemented in C++17, in the style of Stevan Apter in 
[Treetable: a case-study in q](http://archive.vector.org.uk/art10500340).

## Who cares?

A tree is a data structure in which values have parent-child relationships to
each other. They come in many forms.

In most software, trees are implemented like a typical binary tree, where each
node contains its own data and a pointer to each of its children, which are
also nodes. 

Using such a data structure can be complex due to recursion and slow due to
cache behavior in modern systems and the frequent malloc()s. The concept of who
"owns" a tree node in such a system can become quite complex in multi-layered
software.

Apter Trees are much faster, easier to reason about, and easier to implement.

## How it works

An Apter tree is implemented as two same-sized arrays.

One is a vector (array) of data (we'll call it `d`). These correspond to the
things that each node contains.

The other is a vector of parent indices (`p`). The index of an item in the `d`
vector is used as its key, which we will call `c` in the examples below. 

Often, the key/index `c` will just be an int. 

So, if we had a dog family tree in which Coco was the father of Molly and Arca,
and Arca had a son named Cricket, you might have a data structure like:

```
	tree.d = ["Coco", "Molly", "Arca","Cricket"]
	tree.p = [0,0,0,2]
```

A node with a key of `0` whose parent is zero is the root node. Apter trees
require a root node, or the use of `-1` to mean "no parent", which is slightly
less elegant so I'll ignore it.

Computers are very, very fast at manipulating vectors. They're so much faster
than pointer operations that comparisons of big-O notation for an algorithm
don't play out in practice. 

## Operations in psuedocode

The technique is applicable in all languages.  This library is written in C++
but I will use psuedocode to explain how it works.

* Empty tree

tree() = { {d:[], p:[]} }       # some sort of [data,parentidxs] vector

* Number of nodes

```
nodecnt(t) = { len(t.p) }
```

* Keys of all nodes

```
join(x,y) = { flatten(x,y) }    # helper func.. push_back, [], etc.
range(x,y) = { # Q `count`; APL/C++ `iota`; return [x, x+1, x+2, ...y-1]
	i=x; ret=[]; while(i++<y) ret=join(ret,i); return ret
}
keys(t) = { range(0, nodecnt(t)) }
```

* Add an item to the tree:

```
insert(t, val, parentidx) = {
	t.d = join(t.d,val)
	t.p = join(t.p,parentidx)
}
```

* Determine parent of a given node:

Remember, we use the index of a node (called `c`) as its identifier:

```
parentof(t,childidx) = { t.p[childidx] }
```

* Retrieve value of node:

We'll use `c` for `childidx`, a node key, from here on out.

```
data(t,c) = { t.d[c] }
```

* Scan for keys that have a given value:

```
where(vec,val) = { # helper: return indices of vec that contain val
	matches=[]
	for idx,v in vec: if(v==val, {matches=join(matches,idx)})
	return matches
}
search(t,val) = { where(t.d,val) }
```

* Determine children of a node:

```
childnodes(t,c) = { where(t.p,c) }
```

* Retrieve child nodes' values

```
# We're assuming you can index with a vector; otherwise loop required
childdata(t,c) = { data(childnodes(c)) }
```

* Determine leaf nodes (those with no children):

First, build a vector of all the indices. Then remove those indices that are
also in `p`. The psuedocode below is a slow implementation; should be done as a 
single loop.

```
except(x,y) = { # return elements of x except those in y. set subtraction
	ret=[]; 
	for idx,xx in x: if(!xx in y, {ret=join(ret,xx)}); 
	return ret;
}
leaves(t) = { except(keys(t), t.p) }
```

* Determine vector of parents for a given node, or path to node:

Here we keep going up the tree until we can't go any further (ends at 0). We call this form of
iteration `exhaust`. It's called `scan` in K and Q. 

We reverse the result so that it is in `parentA.parentB.parentC.child` order.

```
exhaust(vec,start) = {
	ret=[]; last=x=start
	do {
		last=x
		ret=join(ret,x)
		x=vec[x]
	} until x==last
	return ret
}
parentnodes(t,c) = { reverse(exhaust(t.p, c) }
```

* Determine data for path through tree (i.e., all parents of a node)

```
parentdata(t,c) = { data(parentnodes(t,c)) }
```

* In order traversal

The simplest way is to get the list of leaves, and then determine the path to each. We finally
sort those vectors.

I believe there is a simpler way that can work in a single pass, or at least a single pass
with a sufficiently large stack. I'm still exploring this idea. Let me know if you have any
suggestions.

We're assuming a fairly flexible sort function here which can handle sorting vectors of vectors.

```
all(t) = { sort(each(leaves(t), (c){ parent(t, c) })) }
```

* Delete item 

This can vary depending on your application. A sentinel value like `MAXINT` in
the parent column is probably easiest. Some systems uses `-1` to represent an
empty node if you can spare the sign bit.

## Again, who cares?

I think this is the most elegant implementation of trees I've seen. Given the
right vector operations library it's also by far the shortest, which means you
can easily understand it, fix it. Given common sense it's also the fastest.

Pointers are annoying anyway.

## Origins

I've been lazily trying to figure out who invented this technique. It's so
obvious I would imagine it must have had a name during the vector-oriented 60s
and 70s.

The first full explanation I saw was from Apter as explained above, but it was
also documented widely as early as K3. Here's a version in Q:

```
/ nested directory: use a parent vector, e.g.
/ a
/ b
/  c
/   d
/  e
p:0N 0N 1 2 1 / parent
n:`a`b`c`d`e  / name
c:group p     / children
n p scan 3    / full path
```

I must have read that a hundred times before I internalized its genius.  [See
four other ways to represent trees in K](https://a.kx.com/q/tree.q) each in
about three lines of code.

APL, or at least Dyalog, seems to implement trees in a more traditional way,
using nested boxes: [see here for more](https://dfns.dyalog.com/n_BST.htm).
They do however use a [similar technique for vector
graphs](https://dfns.dyalog.com/n_Graphs.htm).

It appears to be known to J users as seen in [Rosetta Code's tree
implementation in
J](https://rosettacode.org/wiki/Tree_traversal#J:_Alternate_implementation)
(with some illuminating comments).

## Other common tree implementations

Here are some other well known trees. 

None of these do the same thing as an Apter tree, and some are far larger due
to generalizations, but it's still interesting to consider how much code
different styles of trees requires to do simple operations.

* [FreeBSD's kernel tree implementation](https://svnweb.freebsd.org/base/head/sys/sys/tree.h?revision=277642&view=markup)

* [klib's tree](https://github.com/attractivechaos/klib/blob/master/kbtree.h)

* [a tree class in Ruby](https://github.com/ealdent/simple-tree/blob/master/lib/simple_tree.rb)

* [Python declarative tree class](https://github.com/ShuaiW/Python/blob/master/POC/Tree.py)

## Status of this code

I've made a sorta lazy attempt at implementing this in C++ as a way to learn C++17. Not really ready
for use or fully fleshed out. I still have a lot to learn about C++.

## Thanks

Arthur Whitney, Apter, others: inspiration. Dave Linn: proof reading.
