#ifndef STRANDSCENESHAPE_H
#define STRANDSCENESHAPE_H


class StrandSceneShape
{
public:
    //each strand/species will be a StrandSceneShape
    //a strandsceneshape consists of a map<domain_id, strandscenepathitem*>
    //the GUI will have options to add/remove a strand/species from the scene
    //there will be one scene(strandscene) in which you can add/remove items(strands/species)
    StrandSceneShape();
};

#endif // STRANDSCENESHAPE_H
