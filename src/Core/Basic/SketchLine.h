// Shipeng Xu
// billhsu.x@gmail.com

#pragma once
#include <map>
#include <vector>
#include "LineSegment.h"
// Sketch line contains multiple line segments
class SketchLine {
   public:
    // get the sketch line from a line segment
    static SketchLine* lineSegmentToSkectLine(int lineSegmentID);

    static SketchLine* sketchLineFromID(int sketchLineID);

    // Note:
    //    Once added with addSketchLine(), the lineSegments in sketchLine is
    //    fixed.
    static void addSketchLine(SketchLine& sketchLine);
    static void deleteSketchLine(SketchLine& sketchLine);

    static std::map<int, int> lineSegIdTOSketchLineId;
    static std::vector<LineSegment>& getGlobalLineSegments();
    static std::vector<SketchLine>& getGlobalSketchLines() {
        return sketchLines;
    }
    static void clearAllSketchLines() {
        sketchLines.clear();
        deletedLines.clear();
        redoLines.clear();
        lineOperations.clear();
        redoOperations.clear();
        globalLineSegments.clear();
    }
    static void undoLastOperation();
    static void redoLastOperation();

    const LineSegment getLineSegment(const size_t index);
    void setLineSegment(const size_t index, const LineSegment lineSegment);
    void setLineSegmentPoint(const size_t index, const int whichPoint,
                             const Vector3 point);
    size_t getLineSegmentsSize();
    void clearLineSegments();
    void addLineSegment(const LineSegment& line);
    int getID() {
        return ID;
    }

   private:
    enum { OPERATION_ADD_LINE = 1, OPERATION_DELETE_LINE };
    struct SketchLineOperation {
        int sketchLineID;
        int operation;
    };

    static std::vector<SketchLine> sketchLines;
    static std::vector<SketchLine> deletedLines;
    static std::vector<SketchLine> redoLines;
    static std::vector<SketchLineOperation> lineOperations;
    static std::vector<SketchLineOperation> redoOperations;
    static std::vector<LineSegment> globalLineSegments;
    static void updateGlobalLineSegments();
    int ID;
    std::vector<LineSegment> lineSegments;
};