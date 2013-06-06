#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/internal.hpp"

namespace cv
{

#define CC_CASCADE_PARAMS "cascadeParams"
#define CC_STAGE_TYPE     "stageType"
#define CC_FEATURE_TYPE   "featureType"
#define CC_HEIGHT         "height"
#define CC_WIDTH          "width"

#define CC_STAGE_NUM    "stageNum"
#define CC_STAGES       "stages"
#define CC_STAGE_PARAMS "stageParams"

#define CC_BOOST            "BOOST"
#define CC_MAX_DEPTH        "maxDepth"
#define CC_WEAK_COUNT       "maxWeakCount"
#define CC_STAGE_THRESHOLD  "stageThreshold"
#define CC_WEAK_CLASSIFIERS "weakClassifiers"
#define CC_INTERNAL_NODES   "internalNodes"
#define CC_LEAF_VALUES      "leafValues"

#define CC_FEATURES       "features"
#define CC_FEATURE_PARAMS "featureParams"
#define CC_MAX_CAT_COUNT  "maxCatCount"

#define CC_RECTS  "rects"
#define CC_TILTED "tilted"

#define CC_LBP  "LBP"
#define CC_RECT "rect"


#define CV_HAAR_DO_CANNY_PRUNING    1
#define CV_HAAR_SCALE_IMAGE         2
#define CV_HAAR_FIND_BIGGEST_OBJECT 4
#define CV_HAAR_DO_ROUGH_SEARCH     8
//==========================================================================
#define CV_SUM_PTRS( p0, p1, p2, p3, sum, rect, step )                    \
    /* (x, y) */                                                          \
    (p0) = sum + (rect).x + (step) * (rect).y,                            \
    /* (x + w, y) */                                                      \
    (p1) = sum + (rect).x + (rect).width + (step) * (rect).y,             \
    /* (x + w, y) */                                                      \
    (p2) = sum + (rect).x + (step) * ((rect).y + (rect).height),          \
    /* (x + w, y + h) */                                                  \
    (p3) = sum + (rect).x + (rect).width + (step) * ((rect).y + (rect).height)
//
#define CALC_SUM_(p0, p1, p2, p3, offset) \
    ((p0)[offset] - (p1)[offset] - (p2)[offset] + (p3)[offset])   
//----------------------------------------------  LBPEvaluator -------------------------------------
class LBPEvaluator
{
public:
    struct Feature
    {
	public:
		Feature();
        Feature( int x, int y, int _block_w, int _block_h  ) : 
        rect(x, y, _block_w, _block_h) {}
        
        int calc( int offset ) const;
        void updatePtrs( const Mat& sum );
        bool read(const FileNode& node );
        
        Rect rect; // weight and height for block
        const int* p[16]; // fast
    };
    
    LBPEvaluator();
    ~LBPEvaluator();
    
    bool read( const FileNode& node );
    LBPEvaluator* clone() const;
    int getFeatureType() const { return 1; }

    bool setImage(const Mat& image, Size _origWinSize);
    bool setWindow(Point pt);
    
    int operator()(int featureIdx) const
    { return featuresPtr[featureIdx].calc(offset); }
    int calcCat(int featureIdx) const
    { return (*this)(featureIdx); }

        static LBPEvaluator* create( int featureType );
	vector<Feature>* features;
	Feature* featuresPtr; // optimization
private:
    Size origWinSize;
    
    
    Mat sum0, sum;
    Rect normrect;

    int offset;
};    
inline LBPEvaluator::Feature :: Feature()
{
    rect = Rect();
    for( int i = 0; i < 16; i++ )
        p[i] = 0;
}

inline int LBPEvaluator::Feature :: calc( int offset ) const
{
    int cval = CALC_SUM_( p[5], p[6], p[9], p[10], offset );
    
    return (CALC_SUM_( p[0], p[1], p[4], p[5], offset ) >= cval ? 128 : 0) |   // 0
           (CALC_SUM_( p[1], p[2], p[5], p[6], offset ) >= cval ? 64 : 0) |    // 1
           (CALC_SUM_( p[2], p[3], p[6], p[7], offset ) >= cval ? 32 : 0) |    // 2
           (CALC_SUM_( p[6], p[7], p[10], p[11], offset ) >= cval ? 16 : 0) |  // 5
           (CALC_SUM_( p[10], p[11], p[14], p[15], offset ) >= cval ? 8 : 0)|  // 8
           (CALC_SUM_( p[9], p[10], p[13], p[14], offset ) >= cval ? 4 : 0)|   // 7
           (CALC_SUM_( p[8], p[9], p[12], p[13], offset ) >= cval ? 2 : 0)|    // 6
           (CALC_SUM_( p[4], p[5], p[8], p[9], offset ) >= cval ? 1 : 0);
}

inline void LBPEvaluator::Feature :: updatePtrs( const Mat& sum )
{
    const int* ptr = (const int*)sum.data;
    size_t step = sum.step/sizeof(ptr[0]);
    Rect tr = rect;
    CV_SUM_PTRS( p[0], p[1], p[4], p[5], ptr, tr, step );
    tr.x += 2*rect.width;
    CV_SUM_PTRS( p[2], p[3], p[6], p[7], ptr, tr, step );
    tr.y += 2*rect.height;
    CV_SUM_PTRS( p[10], p[11], p[14], p[15], ptr, tr, step );
    tr.x -= 2*rect.width;
    CV_SUM_PTRS( p[8], p[9], p[12], p[13], ptr, tr, step );
}
//----------------------------------------------  predictor functions -------------------------------------
class LBPCascadeClassifier
{
public:
    LBPCascadeClassifier();
    LBPCascadeClassifier( const string& filename );
    ~LBPCascadeClassifier();
	
	bool empty() const;
    bool load( const string& filename );
    bool read( const FileNode& node );
    void detectMultiScale( const Mat& image,
                                   CV_OUT vector<Rect>& objects,
                                   double scaleFactor=1.1,
                                   int minNeighbors=3, int flags=0,
                                   Size minSize=Size(),
                                   Size maxSize=Size() );

    void detectMultiScale( const Mat& image,
                                   CV_OUT vector<Rect>& objects,
                                   vector<int>& rejectLevels,
                                   vector<double>& levelWeights,
                                   double scaleFactor=1.1,
                                   int minNeighbors=3, int flags=0,
                                   Size minSize=Size(),
                                   Size maxSize=Size(),
                                   bool outputRejectLevels=false );


    Size getOriginalWindowSize() const;
    int getFeatureType() const;
    bool setImage( const Mat& );

private:

    bool detectSingleScale( const Mat& image, int stripCount, Size processingRectSize,
                                    int stripSize, int yStep, double factor, vector<Rect>& candidates,
                                    vector<int>& rejectLevels, vector<double>& levelWeights, bool outputRejectLevels=false);

    enum { BOOST = 0 };
    enum { DO_CANNY_PRUNING = 1, SCALE_IMAGE = 2,
           FIND_BIGGEST_OBJECT = 4, DO_ROUGH_SEARCH = 8 };

    friend struct LBPCascadeClassifierInvoker;

    template<class FEval>
    friend int predictCategorical( LBPCascadeClassifier& cascade, LBPEvaluator* featureEvaluator, double& weight);

    template<class FEval>
    friend int predictCategoricalStump( LBPCascadeClassifier& cascade,  LBPEvaluator* featureEvaluator, double& weight);

    bool setImage( LBPEvaluator* feval, const Mat& image);
    virtual int runAt( LBPEvaluator* feval, Point pt, double& weight );

    class Data
    {
    public:
        struct DTreeNode
        {
            int featureIdx;
            int left;
            int right;
			float threshold;
        };

        struct DTree
        {
            int nodeCount;
        };

        struct Stage
        {
            int first;
            int ntrees;
            float threshold;
        };

        bool read(const FileNode &node);

        bool isStumpBased;

        int stageType;
        int featureType;
        int ncategories;
        Size origWinSize;

        vector<Stage> stages;
        vector<DTree> classifiers;
        vector<DTreeNode> nodes;
        vector<float> leaves;
        vector<int> subsets;
    };

    Data data;
    LBPEvaluator* featureEvaluator;

};

template<class FEval>
inline int predictCategorical( LBPCascadeClassifier& cascade, LBPEvaluator* _featureEvaluator, double& sum )
{
    int nstages = (int)cascade.data.stages.size();
    int nodeOfs = 0, leafOfs = 0;
    FEval& featureEvaluator = (FEval&)*_featureEvaluator;
    size_t subsetSize = (cascade.data.ncategories + 31)/32;
    int* cascadeSubsets = &cascade.data.subsets[0];
    float* cascadeLeaves = &cascade.data.leaves[0];
    LBPCascadeClassifier::Data::DTreeNode* cascadeNodes = &cascade.data.nodes[0];
    LBPCascadeClassifier::Data::DTree* cascadeWeaks = &cascade.data.classifiers[0];
    LBPCascadeClassifier::Data::Stage* cascadeStages = &cascade.data.stages[0];
    
    for(int si = 0; si < nstages; si++ )
    {
        LBPCascadeClassifier::Data::Stage& stage = cascadeStages[si];
        int wi, ntrees = stage.ntrees;
        sum = 0;
        
        for( wi = 0; wi < ntrees; wi++ )
        {
            LBPCascadeClassifier::Data::DTree& weak = cascadeWeaks[stage.first + wi];
            int idx = 0, root = nodeOfs;
            do
            {
                LBPCascadeClassifier::Data::DTreeNode& node = cascadeNodes[root + idx];
                int c = featureEvaluator(node.featureIdx);
                const int* subset = &cascadeSubsets[(root + idx)*subsetSize];
                idx = (subset[c>>5] & (1 << (c & 31))) ? node.left : node.right;
            }
            while( idx > 0 );
            sum += cascadeLeaves[leafOfs - idx];
            nodeOfs += weak.nodeCount;
            leafOfs += weak.nodeCount + 1;
        }
        if( sum < stage.threshold )
            return -si;            
    }
    return 1;
}


template<class FEval>
inline int predictCategoricalStump( LBPCascadeClassifier& cascade, LBPEvaluator* _featureEvaluator, double& sum )
{
    int nstages = (int)cascade.data.stages.size();
    int nodeOfs = 0, leafOfs = 0;
    FEval& featureEvaluator = (FEval&)*_featureEvaluator;
    size_t subsetSize = (cascade.data.ncategories + 31)/32;
    int* cascadeSubsets = &cascade.data.subsets[0];
    float* cascadeLeaves = &cascade.data.leaves[0];
    LBPCascadeClassifier::Data::DTreeNode* cascadeNodes = &cascade.data.nodes[0];
    LBPCascadeClassifier::Data::Stage* cascadeStages = &cascade.data.stages[0];

    for( int si = 0; si < nstages; si++ )
    {
        LBPCascadeClassifier::Data::Stage& stage = cascadeStages[si];
        int wi, ntrees = stage.ntrees;
        sum = 0;

        for( wi = 0; wi < ntrees; wi++ )
        {
            LBPCascadeClassifier::Data::DTreeNode& node = cascadeNodes[nodeOfs];
            int c = featureEvaluator(node.featureIdx);
            const int* subset = &cascadeSubsets[nodeOfs*subsetSize];
            sum += cascadeLeaves[ subset[c>>5] & (1 << (c & 31)) ? leafOfs : leafOfs+1];
            nodeOfs++;
            leafOfs += 2;
        }
        if( sum < stage.threshold )
            return -si;            

    }
    return 1;
}
}

