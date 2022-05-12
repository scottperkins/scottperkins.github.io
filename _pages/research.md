---
layout: single
author_profile: true
title: Research
permalink: /research
---

At the University of Illinois at Urbana-Champaign, I work under Dr. Nicolas Yunes in the relativity group and iCASU, where I have a fellowship through the Center for AstroPhysical Surveys. I am researching how general relativity, our best theory of gravitational interaction to date, can be extended to try and solve some of physics' long standing problems. By altering the fundamental nature of how bodies interact gravitationally, solutions might be found that can address issues like the expansion of the universe or might help us bridge the gap between our best theory of microscopic systems, quantum mechanics.

While plenty of theories aim to solve some of these issues in physics, only theories that are consistent with observable data are relevant. Experiments that can actually test these types of theories at the required energy scales are only highly relativistic, astrophysical systems. Observations are focused on binary neutron stars and binary black holes. The enormous amounts of energy released by these inspiralling and colliding bodies allows us to look for tell-tale signs of these extensions to general relativity. These signals come both in the form of electromagnetic radiations (seen by conventional telescopes) and through gravitational waves (seen by new detectors like LIGO and Virgo). My research focuses on the latter, sifting through gravitational wave data looking for evidence of new physics.


## Past Work

<p>
In online databases:
<span class="archive__item-title">
<a href="https://arxiv.org/a/{{ site.author.arXiv }}"><i class="ai ai-fw ai-arxiv" aria-hidden="true"></i> arXiv</a>,
<a href="https://orcid.org/{{ site.author.orcid }}"><i class="ai ai-fw ai-orcid" aria-hidden="true"></i> ORCiD</a>,
<a href="https://scholar.google.com/citations?user={{ site.author.google-scholar }}"><i class="ai ai-fw ai-google-scholar" aria-hidden="true"></i> Google Scholar</a>,
and
<a href="https://inspirehep.net/search?p=exactauthor%3A{{ site.author.inspire }}"><i class="ai ai-fw ai-inspire" aria-hidden="true"></i> INSPIRE</a></span>.
<br>
I also curate all my publications below for some attempt at
consistency.
</p>

{% include base_path %}
{% capture written_year %}'None'{% endcapture %}
{% for post in site.posts %}
  {% if post.research %}
    {% capture year %}{{ post.date | date: '%Y' }}{% endcapture %}
    {% if year != written_year %}
      <h2 id="{{ year | slugify }}" class="archive__subtitle"><a href="#{{ year | slugify }}">#{{ year }}</a></h2>
      {% capture written_year %}{{ year }}{% endcapture %}
    {% endif %}
    {% include archive-single.html %}
  {% endif %}
{% endfor %}
