# Python script to parse a KiCad .kicad_pcb file using kiutils and prepare 3D rendering data.

from kiutils.board import Board
from kiutils.items.common import Position
from kiutils.items.brditems import Via, Segment
import math
import json

def parse_board_to_geometry(file_path):
    """
    Parse a KiCad PCB file (.kicad_pcb) and extract geometry for 3D rendering.
    Returns a dictionary with board outline, holes, traces, and component placeholders.
    """
    # Load the KiCad board file using kiutils (supports KiCad v6+ .kicad_pcb):contentReference[oaicite:0]{index=0}:contentReference[oaicite:1]{index=1}.
    board = Board.from_file(file_path)

    # --- Board Outline and Holes (Edge.Cuts layer) ---
    # The Edge.Cuts layer defines the board outline and board cutouts (holes).
    outline_vertices = []
    outline_indices = []
    hole_centers = []
    hole_radii = []
    for item in board.graphicItems:
        layer = getattr(item, 'layer', None)
        if layer == 'Edge.Cuts':
            if item.__class__.__name__ == 'GrLine':
                start = item.start; end = item.end
                outline_vertices.append((start.X, start.Y))
                outline_vertices.append((end.X, end.Y))
            elif item.__class__.__name__ == 'GrPoly':
                coords = [(pt.X, pt.Y) for pt in item.coordinates]
                outline_vertices.extend(coords)
            elif item.__class__.__name__ == 'GrCircle':
                center = item.center; edge = item.end
                # Compute circle radius from center to edge point
                r = math.hypot(edge.X - center.X, edge.Y - center.Y)
                hole_centers.append((center.X, center.Y))
                hole_radii.append(r)
            elif item.__class__.__name__ == 'GrRect':
                start = item.start; end = item.end
                outline_vertices.append((start.X, start.Y))
                outline_vertices.append((start.X, end.Y))
                outline_vertices.append((end.X, end.Y))
                outline_vertices.append((end.X, start.Y))
            elif item.__class__.__name__ == 'GrArc':
                start = item.start; end = item.end
                outline_vertices.append((start.X, start.Y))
                outline_vertices.append((end.X, end.Y))
    # (Outline vertices would be processed into a mesh with indices; omitted here for brevity.)

    # --- Copper Traces ---
    # Extract copper trace segments from board.traceItems:contentReference[oaicite:2]{index=2}.
    trace_segments = []
    for item in board.traceItems:
        if isinstance(item, Segment):
            x1, y1 = item.start.X, item.start.Y
            x2, y2 = item.end.X, item.end.Y
            width = item.width
            trace_segments.append({"start": (x1, y1), "end": (x2, y2), "width": width})
    # (Arcs/Vias could be handled similarly if needed.)

    # --- Component Placeholders ---
    # Board.footprints holds all placed components (footprints):contentReference[oaicite:3]{index=3}.
    components = []
    for fp in board.footprints:
        ref = ""
        for item in fp.graphicItems:
            if getattr(item, 'type', None) == 'reference':
                ref = item.text if hasattr(item, 'text') else ""
                break
        pos = fp.position
        comp_x, comp_y = (pos.X if pos else 0), (pos.Y if pos else 0)
        angle = getattr(pos, 'angle', 0) if pos else 0
        model = 'Component'
        if ref:
            if ref.startswith('R'):
                model = 'Resistor'
            elif ref.startswith('C'):
                model = 'Capacitor'
            elif ref.startswith('U'):
                model = 'IC'
        components.append({"ref": ref, "model": model,
                           "position": (comp_x, comp_y, 0), "rotation": angle})

    # --- Drill Holes from Footprints ---
    holes = []
    for fp in board.footprints:
        pos = fp.position
        if pos is None:
            continue
        fp_x, fp_y = pos.X, pos.Y
        angle = getattr(pos, 'angle', 0) or 0
        cos_a = math.cos(math.radians(angle))
        sin_a = math.sin(math.radians(angle))
        for pad in fp.pads:
            if pad.drill is not None:
                pad_x, pad_y = pad.position.X, pad.position.Y
                # Apply footprint rotation to pad offset
                rot_x = pad_x * cos_a - pad_y * sin_a
                rot_y = pad_x * sin_a + pad_y * cos_a
                hole_center = (fp_x + rot_x, fp_y + rot_y)
                holes.append({"center": hole_center, "diameter": pad.drill.diameter})

    # Assemble geometry data
    geometry = {
        "outline": {"vertices": outline_vertices, "indices": outline_indices},
        "holes": {
            "edge_centers": hole_centers,
            "edge_radii": hole_radii,
            "footprint_holes": holes
        },
        "traces": trace_segments,
        "components": components
    }
    return geometry

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("Usage: python parse_kicad.py board_file.kicad_pcb")
    else:
        data = parse_board_to_geometry(sys.argv[1])
        print(json.dumps(data, indent=2))

