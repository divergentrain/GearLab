#import "@preview/cetz:0.4.1"
#set page(height: auto, width: auto, fill: none)

#let fa = 60deg
#let ra = 30deg
#let fao = 0.5
#let rao = -0.5
#let pa = 45deg
#let coneLength = 3.0
#let coneStart = 1.0

#cetz.canvas({
  import cetz.draw: *

  // Set base styles
  set-style(stroke: 0.4pt, grid: (stroke: gray + 0.2pt))
  grid((-5,-6), (5,2))
  line((0,2),(0,-6))  // Z axis
  line((-5,0),(5,0))  // X axis
  content((0,2.3), [Z])
  content((5.3,0), [X])

  // ------------------------
  // Pitch Cone (starts at origin)
  // ------------------------
  set-style(stroke: blue + 0.7pt)
  line((0,0), (coneLength*calc.tan(pa), -coneLength))
  line((0,0), (-coneLength*calc.tan(pa), -coneLength))
  content((coneLength*calc.tan(pa) + 0.3, -coneLength - 0.3), [Pitch Cone])

  // ------------------------
  // Face Cone (offset by fao)
  // ------------------------
  set-style(stroke: green + 0.7pt)
  let faceBaseY = fao
  let faceEndY = -coneLength + fao
  let faceStartY = -coneStart + fao
  line((0, faceBaseY), (coneLength*calc.tan(fa), faceEndY))
  line((0, faceBaseY), (-coneLength*calc.tan(fa), faceEndY))
  content((coneLength*calc.tan(fa) + 0.3, faceEndY - 0.3), [Face Cone (FA)])

  // ------------------------
  // Root Cone (offset by rao)
  // ------------------------
  set-style(stroke: red + 0.7pt)
  let rootBaseY = rao
  let rootEndY = -coneLength + rao
  let rootStartY = -coneStart + rao
  line((0, rootBaseY), (coneLength*calc.tan(ra), rootEndY))
  line((0, rootBaseY), (-coneLength*calc.tan(ra), rootEndY))
  content((coneLength*calc.tan(ra), rootEndY - 0.3), [Root Cone (RA)])

  // ------------------------
  // Highlight region between face & root cone
  // ------------------------
  let alpha_gray = color.linear-rgb(100,100,100,30%)
  set-style(stroke: none, fill: alpha_gray)
  line(
    (-coneStart*calc.tan(fa), faceStartY),
    (-coneStart*calc.tan(ra), rootStartY),
    (-coneLength*calc.tan(ra), rootEndY),
    (-coneLength*calc.tan(fa), faceEndY), close: true
  )

  line(
    (coneStart*calc.tan(fa), faceStartY),
    (coneStart*calc.tan(ra), rootStartY),
    (coneLength*calc.tan(ra), rootEndY),
    (coneLength*calc.tan(fa), faceEndY), close: true
  )
  // ------------------------
  // Offset labels
  // ------------------------
  set-style(stroke: black + 0.4pt, mark: (end: ">"))
  line((3.1, fao/2), (0, fao))
  content((4.0, fao/2), [FA Offset])

  line((3.5, -1), (0, rao))
  content((4.5, -1), [RA Offset])
  
  // ------------------------
  // Teeth labels
  // ------------------------
  let coneMid = ((coneLength - coneStart) / 2) + fao
  line((-4,-0.5),(-coneMid*calc.tan(fa), -coneMid))
  content((-4.2,-0.2),[Gear tooth])
})




