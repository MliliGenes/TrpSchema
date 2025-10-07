/**
 * TrpJSON Parser Website - Interactive JavaScript
 * Modern cyberpunk-themed interactions and animations
 */

class TrpJSONWebsite {
    constructor() {
        this.isLoaded = false;
        this.observers = [];
        this.animationQueue = [];
        this.init();
    }

    init() {
        // Wait for DOM to be ready
        if (document.readyState === 'loading') {
            document.addEventListener('DOMContentLoaded', () => this.onDOMReady());
        } else {
            this.onDOMReady();
        }
    }

    onDOMReady() {
        console.log('🚀 TrpJSON Website Initializing...');
        
        // Initialize all components
        this.setupInteractiveElements();
        this.setupCodeHighlighting();
        this.setupBenchmarkAnimations();
        this.setupGitHubIntegration();
        this.setupParticleEffects();
        this.setupKeyboardShortcuts();
        this.setupProgressBars();
        this.setupTypewriterEffect();
        
        // Mark as loaded
        this.isLoaded = true;
        console.log('✅ TrpJSON Website Ready!');
    }



    /**
     * Feature card hover effects and animations
     */
    animateFeatureCard(card) {
        const icon = card.querySelector('.feature-icon');
        const title = card.querySelector('.feature-title');
        
        // Stagger animations
        setTimeout(() => {
            if (icon) icon.style.transform = 'scale(1) rotate(0deg)';
        }, 100);
        
        setTimeout(() => {
            if (title) title.style.opacity = '1';
        }, 200);

        // Add interactive hover effects
        card.addEventListener('mouseenter', () => {
            if (icon) {
                icon.style.transform = 'scale(1.1) rotate(10deg)';
                icon.style.boxShadow = '0 8px 32px rgba(0, 255, 136, 0.4)';
            }
        });

        card.addEventListener('mouseleave', () => {
            if (icon) {
                icon.style.transform = 'scale(1) rotate(0deg)';
                icon.style.boxShadow = '0 4px 16px rgba(0, 255, 136, 0.2)';
            }
        });
    }

    /**
     * Benchmark card animations with counter effect
     */
    animateBenchmarkCard(card) {
        const valueElement = card.querySelector('.benchmark-value');
        const progressBar = card.querySelector('.progress-fill');
        
        if (valueElement) {
            const finalValue = valueElement.textContent;
            const isNumeric = /^\d+(\.\d+)?/.test(finalValue);
            
            if (isNumeric) {
                this.animateCounter(valueElement, parseFloat(finalValue), finalValue);
            }
        }

        // Animate progress bar
        if (progressBar) {
            const targetWidth = progressBar.style.width;
            progressBar.style.width = '0%';
            
            setTimeout(() => {
                progressBar.style.width = targetWidth;
            }, 300);
        }
    }

    /**
     * Stat item counter animations
     */
    animateStatItem(item) {
        const numberElement = item.querySelector('.stat-number');
        if (numberElement) {
            const text = numberElement.textContent;
            const number = parseFloat(text);
            
            if (!isNaN(number)) {
                this.animateCounter(numberElement, number, text);
            }
        }
    }

    /**
     * Counter animation for numbers
     */
    animateCounter(element, target, originalText) {
        const duration = 2000; // 2 seconds
        const steps = 60;
        const increment = target / steps;
        let current = 0;
        let step = 0;

        const timer = setInterval(() => {
            current += increment;
            step++;
            
            if (step >= steps) {
                element.textContent = originalText;
                clearInterval(timer);
            } else {
                // Format based on original text
                if (originalText.includes('MB/s')) {
                    element.textContent = `${Math.floor(current)}MB/s`;
                } else if (originalText.includes('~')) {
                    element.textContent = `~${current.toFixed(1)}x`;
                } else if (originalText.includes('.')) {
                    element.textContent = current.toFixed(2);
                } else {
                    element.textContent = Math.floor(current);
                }
            }
        }, duration / steps);
    }

    /**
     * Interactive elements setup
     */
    setupInteractiveElements() {
        // Smooth scrolling for anchor links
        document.querySelectorAll('a[href^="#"]').forEach(anchor => {
            anchor.addEventListener('click', (e) => {
                e.preventDefault();
                const target = document.querySelector(anchor.getAttribute('href'));
                if (target) {
                    target.scrollIntoView({
                        behavior: 'smooth',
                        block: 'start'
                    });
                }
            });
        });

        // Button hover effects
        document.querySelectorAll('.btn').forEach(btn => {
            btn.addEventListener('mouseenter', () => {
                btn.style.transform = 'translateY(-2px) scale(1.02)';
            });
            
            btn.addEventListener('mouseleave', () => {
                btn.style.transform = 'translateY(0) scale(1)';
            });
        });

        // Badge interactions
        document.querySelectorAll('.badge').forEach(badge => {
            badge.addEventListener('click', () => {
                this.createFloatingText(badge, '✨');
            });
        });
    }

    /**
     * Code highlighting and copy functionality
     */
    setupCodeHighlighting() {
        // Add copy buttons to code blocks
        document.querySelectorAll('pre').forEach(pre => {
            if (!pre.querySelector('.copy-btn')) {
                const copyBtn = this.createCopyButton();
                pre.style.position = 'relative';
                pre.appendChild(copyBtn);
            }
        });

        // Syntax highlighting for JSON
        // this.highlightJSON();
    }

    /**
     * Create copy button for code blocks
     */
    createCopyButton() {
        const btn = document.createElement('button');
        btn.className = 'copy-btn';
        btn.innerHTML = '<i class="fas fa-copy"></i>';
        btn.style.cssText = `
            position: absolute;
            top: 1rem;
            right: 1rem;
            background: var(--surface-variant);
            border: 1px solid var(--border);
            color: var(--text-primary);
            padding: 0.5rem;
            border-radius: var(--radius-sm);
            cursor: pointer;
            opacity: 0;
            transition: var(--transition);
            z-index: 10;
        `;

        btn.addEventListener('click', async () => {
            const code = btn.parentElement.querySelector('code');
            const text = code ? code.textContent : btn.parentElement.textContent;
            
            try {
                await navigator.clipboard.writeText(text.trim());
                btn.innerHTML = '<i class="fas fa-check"></i>';
                btn.style.background = 'var(--success)';
                
                setTimeout(() => {
                    btn.innerHTML = '<i class="fas fa-copy"></i>';
                    btn.style.background = 'var(--surface-variant)';
                }, 2000);
            } catch (err) {
                console.error('Copy failed:', err);
            }
        });

        // Show/hide on parent hover
        const parent = btn.parentElement;
        if (parent) {
            parent.addEventListener('mouseenter', () => {
                btn.style.opacity = '1';
            });
            
            parent.addEventListener('mouseleave', () => {
                btn.style.opacity = '0';
            });
        }

        return btn;
    }

    /**
     * Enhanced JSON syntax highlighting
     */
    highlightJSON() {
        document.querySelectorAll('.json-output code').forEach(code => {
            if (!code.dataset.highlighted) {
                let html = code.innerHTML;
                
                // Enhance JSON highlighting with better colors
                html = html.replace(/"([^"]+)":/g, '<span class="json-key">"$1":</span>');
                html = html.replace(/"([^"]+)"(?!:)/g, '<span class="json-string">"$1"</span>');
                html = html.replace(/\b(\d+\.?\d*)\b/g, '<span class="json-number">$1</span>');
                html = html.replace(/\b(true|false)\b/g, '<span class="json-boolean">$1</span>');
                html = html.replace(/\bnull\b/g, '<span class="json-null">null</span>');
                
                code.innerHTML = html;
                code.dataset.highlighted = 'true';
            }
        });
    }

    /**
     * Benchmark progress bar animations
     */
    setupBenchmarkAnimations() {
        const progressBars = document.querySelectorAll('.progress-fill');
        
        progressBars.forEach((bar, index) => {
            const targetWidth = bar.style.width;
            bar.style.width = '0%';
            
            // Animate when visible
            const observer = new IntersectionObserver((entries) => {
                entries.forEach(entry => {
                    if (entry.isIntersecting) {
                        setTimeout(() => {
                            bar.style.width = targetWidth;
                        }, index * 200);
                        observer.unobserve(entry.target);
                    }
                });
            });
            
            observer.observe(bar);
        });
    }

    /**
     * GitHub integration for avatar and profile data
     */
    async setupGitHubIntegration() {
        const avatar = document.getElementById('github-avatar');
        if (!avatar) return;

        try {
            const response = await fetch('https://api.github.com/users/MliliGenes');
            const userData = await response.json();
            
            if (userData.avatar_url) {
                // Preload avatar with fade-in effect
                const img = new Image();
                img.onload = () => {
                    avatar.src = userData.avatar_url;
                    avatar.style.opacity = '0';
                    avatar.style.transition = 'opacity 0.5s ease';
                    
                    setTimeout(() => {
                        avatar.style.opacity = '1';
                    }, 100);
                };
                img.src = userData.avatar_url;
                
                // Add profile stats if available
                this.addGitHubStats(userData);
            }
        } catch (error) {
            console.log('GitHub API unavailable, using fallback');
            this.setFallbackAvatar(avatar);
        }
    }

    /**
     * Add GitHub stats to credits section
     */
    addGitHubStats(userData) {
        const creditsInfo = document.querySelector('.credits-info');
        if (creditsInfo && userData.public_repos !== undefined) {
            const statsEl = document.createElement('div');
            statsEl.className = 'github-stats';
            statsEl.style.cssText = `
                margin-top: 1rem;
                padding-top: 1rem;
                border-top: 1px solid var(--border);
                font-size: 0.875rem;
                color: var(--text-secondary);
            `;
            
            statsEl.innerHTML = `
                <div style="display: flex; gap: 1rem; flex-wrap: wrap;">
                    <span><i class="fas fa-code-branch"></i> ${userData.public_repos} repos</span>
                    <span><i class="fas fa-users"></i> ${userData.followers} followers</span>
                </div>
            `;
            
            creditsInfo.appendChild(statsEl);
        }
    }

    /**
     * Fallback avatar with canvas-generated image
     */
    setFallbackAvatar(avatar) {
        const canvas = document.createElement('canvas');
        canvas.width = 200;
        canvas.height = 200;
        const ctx = canvas.getContext('2d');
        
        // Create cyberpunk-style gradient
        const gradient = ctx.createRadialGradient(100, 100, 20, 100, 100, 100);
        gradient.addColorStop(0, '#00ff88');
        gradient.addColorStop(0.5, '#00cc6a');
        gradient.addColorStop(1, '#009955');
        
        ctx.fillStyle = gradient;
        ctx.fillRect(0, 0, 200, 200);
        
        // Add initials
        ctx.fillStyle = '#0a0a0f';
        ctx.font = 'bold 80px system-ui';
        ctx.textAlign = 'center';
        ctx.textBaseline = 'middle';
        ctx.fillText('SM', 100, 100);
        
        avatar.src = canvas.toDataURL();
    }

    /**
     * Particle effects for background enhancement
     */
    setupParticleEffects() {
        const particleContainer = document.createElement('div');
        particleContainer.className = 'particles';
        particleContainer.style.cssText = `
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            pointer-events: none;
            z-index: 1;
            overflow: hidden;
        `;
        
        document.body.appendChild(particleContainer);
        
        // Create floating particles
        for (let i = 0; i < 15; i++) {
            setTimeout(() => this.createParticle(particleContainer), i * 200);
        }
    }

    /**
     * Create individual floating particles
     */
    createParticle(container) {
        const particle = document.createElement('div');
        particle.style.cssText = `
            position: absolute;
            width: 4px;
            height: 4px;
            background: var(--primary);
            border-radius: 50%;
            opacity: 0.3;
            animation: float ${5 + Math.random() * 5}s infinite linear;
        `;
        
        // Random starting position
        particle.style.left = Math.random() * 100 + '%';
        particle.style.top = '100%';
        
        // Add floating animation
        const style = document.createElement('style');
        style.textContent = `
            @keyframes float {
                0% {
                    transform: translateY(0) translateX(0);
                    opacity: 0;
                }
                10% { opacity: 0.3; }
                90% { opacity: 0.3; }
                100% {
                    transform: translateY(-100vh) translateX(${Math.random() * 100 - 50}px);
                    opacity: 0;
                }
            }
        `;
        
        if (!document.querySelector('#particle-styles')) {
            style.id = 'particle-styles';
            document.head.appendChild(style);
        }
        
        container.appendChild(particle);
        
        // Remove particle after animation
        setTimeout(() => {
            if (particle.parentNode) {
                particle.parentNode.removeChild(particle);
            }
            // Create new particle to maintain count
            setTimeout(() => this.createParticle(container), Math.random() * 3000);
        }, 10000);
    }

    /**
     * Keyboard shortcuts for enhanced UX
     */
    setupKeyboardShortcuts() {
        document.addEventListener('keydown', (e) => {
            // Ctrl/Cmd + K to focus search (if implemented)
            if ((e.ctrlKey || e.metaKey) && e.key === 'k') {
                e.preventDefault();
                console.log('Search shortcut pressed');
            }
            
            // Escape to close modals (if any)
            if (e.key === 'Escape') {
                this.closeModals();
            }
            
            // Arrow keys for demo navigation
            if (e.key === 'ArrowLeft' || e.key === 'ArrowRight') {
                this.navigateDemo(e.key);
            }
        });
    }

    /**
     * Progress bars with enhanced animations
     */
    setupProgressBars() {
        const progressBars = document.querySelectorAll('.progress-fill');
        
        progressBars.forEach(bar => {
            // Add glow effect on hover
            bar.addEventListener('mouseenter', () => {
                bar.style.boxShadow = '0 0 20px var(--primary)';
                bar.style.filter = 'brightness(1.2)';
            });
            
            bar.addEventListener('mouseleave', () => {
                bar.style.boxShadow = 'none';
                bar.style.filter = 'brightness(1)';
            });
        });
    }

    /**
     * Typewriter effect for hero subtitle
     */
    setupTypewriterEffect() {
        const subtitle = document.querySelector('.hero .subtitle');
        if (subtitle) {
            const text = subtitle.textContent;
            subtitle.textContent = '';
            subtitle.style.opacity = '1';
            
            let i = 0;
            const typeWriter = () => {
                if (i < text.length) {
                    subtitle.textContent += text.charAt(i);
                    i++;
                    setTimeout(typeWriter, 30);
                } else {
                    // Add blinking cursor
                    const cursor = document.createElement('span');
                    cursor.textContent = '|';
                    cursor.style.animation = 'blink 1s infinite';
                    subtitle.appendChild(cursor);
                    
                    // Add blink animation
                    if (!document.querySelector('#cursor-blink')) {
                        const style = document.createElement('style');
                        style.id = 'cursor-blink';
                        style.textContent = `
                            @keyframes blink {
                                0%, 50% { opacity: 1; }
                                51%, 100% { opacity: 0; }
                            }
                        `;
                        document.head.appendChild(style);
                    }
                }
            };
            
            // Start typewriter after a delay
            setTimeout(typeWriter, 500);
        }
    }

    /**
     * Create floating text effect
     */
    createFloatingText(element, text) {
        const floating = document.createElement('div');
        floating.textContent = text;
        floating.style.cssText = `
            position: absolute;
            color: var(--primary);
            font-weight: bold;
            pointer-events: none;
            z-index: 1000;
            animation: floatUp 1s ease-out forwards;
        `;
        
        const rect = element.getBoundingClientRect();
        floating.style.left = rect.left + rect.width / 2 + 'px';
        floating.style.top = rect.top + 'px';
        
        document.body.appendChild(floating);
        
        // Add float animation
        if (!document.querySelector('#float-up')) {
            const style = document.createElement('style');
            style.id = 'float-up';
            style.textContent = `
                @keyframes floatUp {
                    0% { transform: translateY(0); opacity: 1; }
                    100% { transform: translateY(-50px); opacity: 0; }
                }
            `;
            document.head.appendChild(style);
        }
        
        setTimeout(() => {
            if (floating.parentNode) {
                floating.parentNode.removeChild(floating);
            }
        }, 1000);
    }

    /**
     * Close any open modals
     */
    closeModals() {
        // Implementation for future modal components
        console.log('Closing modals...');
    }

    /**
     * Navigate demo panels with arrow keys
     */
    navigateDemo(direction) {
        // Implementation for demo navigation
        console.log('Demo navigation:', direction);
    }

    /**
     * Cleanup observers and event listeners
     */
    destroy() {
        this.observers.forEach(observer => observer.disconnect());
        console.log('🧹 TrpJSON Website cleaned up');
    }
}

// Initialize the website when DOM is ready
const trpJSONSite = new TrpJSONWebsite();

// Make it globally available for debugging
window.trpJSONSite = trpJSONSite;

// Add some CSS animations dynamically
const dynamicStyles = document.createElement('style');
dynamicStyles.textContent = `
    
    .copy-btn:hover {
        opacity: 1 !important;
        transform: scale(1.1);
        box-shadow: 0 4px 12px rgba(0, 255, 136, 0.3);
    }
    
    .github-stats {
        animation: fadeInUp 0.5s ease-out 0.3s both;
    }
    
    @keyframes fadeInUp {
        from {
            opacity: 0;
            transform: translateY(20px);
        }
        to {
            opacity: 1;
            transform: translateY(0);
        }
    }
`;

document.head.appendChild(dynamicStyles);

// Performance monitoring
console.log('⚡ TrpJSON Website Performance Monitor Active');
window.addEventListener('load', () => {
    console.log(`🎉 Page loaded in ${performance.now().toFixed(2)}ms`);
});

// Add some Easter eggs
let konamiCode = [];
const konamiSequence = [38, 38, 40, 40, 37, 39, 37, 39, 66, 65]; // ↑↑↓↓←→←→BA

document.addEventListener('keydown', (e) => {
    konamiCode.push(e.keyCode);
    konamiCode = konamiCode.slice(-konamiSequence.length);
    
    if (konamiCode.join(',') === konamiSequence.join(',')) {
        console.log('🎮 Konami Code activated!');
        document.body.style.filter = 'hue-rotate(180deg)';
        setTimeout(() => {
            document.body.style.filter = 'none';
        }, 3000);
        konamiCode = [];
    }
});

console.log('🎯 Easter egg hint: Try the Konami Code! ↑↑↓↓←→←→BA');
